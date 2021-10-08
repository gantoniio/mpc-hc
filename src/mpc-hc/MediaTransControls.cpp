/*
* (C) 2002-2021 see Authors.txt
*
* This file is part of MPC-HC.
*
* MPC-HC is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* MPC-HC is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "stdafx.h"
#include "MainFrm.h"
#include "MediaTransControls.h"
#include "shcore.h"
#include "PathUtils.h"

#pragma comment(lib, "RuntimeObject.lib")
#pragma comment(lib, "ShCore.lib")

/// #include <SystemMediaTransportControlsInterop.h>
#include <wrl.h>
#include <string>

/// The file content of SystemMediaTransportControlsInterop.h
#ifndef ISystemMediaTransportControlsInterop
EXTERN_C const IID IID_ISystemMediaTransportControlsInterop;
MIDL_INTERFACE("ddb0472d-c911-4a1f-86d9-dc3d71a95f5a")
ISystemMediaTransportControlsInterop : public IInspectable{
 public:
  virtual HRESULT STDMETHODCALLTYPE GetForWindow(
      /* [in] */ __RPC__in HWND appWindow,
      /* [in] */ __RPC__in REFIID riid,
      /* [iid_is][retval][out] */
      __RPC__deref_out_opt void** mediaTransportControl) = 0;
};
#  endif

using namespace Windows::Foundation;
using namespace ABI::Windows::Media;
using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

inline bool IsWindowsVersionOrLater(uint32_t aVersion) {
    static uint32_t minVersion(0);
    static uint32_t maxVersion(UINT32_MAX);

    if (minVersion >= aVersion) {
        return true;
    }

    if (aVersion >= maxVersion) {
        return false;
    }

    OSVERSIONINFOEXW info;
    ZeroMemory(&info, sizeof(OSVERSIONINFOEXW));
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
    info.dwMajorVersion = aVersion >> 24;
    info.dwMinorVersion = (aVersion >> 16) & 0xFF;
    info.wServicePackMajor = (aVersion >> 8) & 0xFF;
    info.wServicePackMinor = aVersion & 0xFF;

    DWORDLONG conditionMask = 0;
    VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_SERVICEPACKMINOR, VER_GREATER_EQUAL);

    if (VerifyVersionInfoW(&info,
        VER_MAJORVERSION | VER_MINORVERSION |
        VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
        conditionMask)) {
        minVersion = aVersion;
        return true;
    }

    maxVersion = aVersion;
    return false;
}

bool MediaTransControls::Init(CMainFrame* main) {
    /// Windows 8.1 or later is required
    if (!IsWindowsVersionOrLater(0x06030000ul)) return false;
    CComPtr<ISystemMediaTransportControlsInterop> op;
    HRESULT ret;
    if ((ret = GetActivationFactory(HStringReference(L"Windows.Media.SystemMediaTransportControls").Get(), &op)) != S_OK) {
        this->controls = nullptr;
        return false;
    }
    if ((ret = op->GetForWindow(main->GetSafeHwnd(), IID_PPV_ARGS(&this->controls))) != S_OK) {
        this->controls = nullptr;
        return false;
    }
    this->controls->put_PlaybackStatus(MediaPlaybackStatus::MediaPlaybackStatus_Closed);
    ret = this->controls->get_DisplayUpdater(&this->updater);
    ASSERT(ret == S_OK);
    ret = this->updater->put_Type(MediaPlaybackType::MediaPlaybackType_Video);
    ASSERT(ret == S_OK);
    ret = this->updater->get_VideoProperties(&this->video);
    ASSERT(ret == S_OK);
    return true;
}

void MediaTransControls::stop() {
    if (this->controls != nullptr) {
        this->controls->put_IsEnabled(false);
        this->controls->put_PlaybackStatus(MediaPlaybackStatus::MediaPlaybackStatus_Closed);
        if (this->updater) this->updater->ClearAll();
    }
}

void MediaTransControls::play() {
    if (this->controls != nullptr) {
        this->controls->put_IsEnabled(true);
        this->controls->put_IsPlayEnabled(true);
        this->controls->put_PlaybackStatus(MediaPlaybackStatus::MediaPlaybackStatus_Playing);
    }
}

template <typename T>
bool AwaitForIAsyncOperation(CComPtr<ABI::Windows::Foundation::IAsyncOperation<T>> io) {
    CComPtr<ABI::Windows::Foundation::IAsyncInfo> info;
    ABI::Windows::Foundation::AsyncStatus status;
    HRESULT ret;
    info = io;
    while (true) {
        if ((ret = info->get_Status(&status)) != S_OK) {
            return false;
        }
        if (status != ABI::Windows::Foundation::AsyncStatus::Started) {
            if (status == ABI::Windows::Foundation::AsyncStatus::Completed) return true;
            return false;
        }
        Sleep(10);
    }
}

void MediaTransControls::loadThumbnail(CString fn) {
    if (fn.IsEmpty() || !updater) return;
    if (PathUtils::IsURL(fn)) return loadThumbnailFromUrl(fn);
    HRESULT ret;
    CComPtr<IStorageFileStatics> sfs;
    if ((ret = GetActivationFactory(HStringReference(RuntimeClass_Windows_Storage_StorageFile).Get(), &sfs)) != S_OK) {
        return;
    }
    CComPtr<ABI::Windows::Foundation::IAsyncOperation<StorageFile*>> af;
    if ((ret = sfs->GetFileFromPathAsync(HStringReference(fn.GetString()).Get(), &af)) != S_OK) {
        return;
    }
    /// Present file
    CComPtr<IStorageFile> f;
    if (!AwaitForIAsyncOperation(af)) {
        return;
    }
    if ((ret = af->GetResults(&f)) != S_OK) {
        return;
    }
    CComPtr<Streams::IRandomAccessStreamReferenceStatics> rasrs;
    if ((ret = GetActivationFactory(HStringReference(RuntimeClass_Windows_Storage_Streams_RandomAccessStreamReference).Get(), &rasrs)) != S_OK) {
        return;
    }
    CComPtr<Streams::IRandomAccessStreamReference> stream;
    if ((ret = rasrs->CreateFromFile(f, &stream)) != S_OK) {
        return;
    }
    updater->put_Thumbnail(stream);
}

void MediaTransControls::loadThumbnail(BYTE* content, size_t size) {
    if (!content || !size || !updater) return;
    ComPtr<Streams::IRandomAccessStream> s;
    HRESULT ret;
    if ((ret = ActivateInstance(HStringReference(RuntimeClass_Windows_Storage_Streams_InMemoryRandomAccessStream).Get(), s.GetAddressOf())) != S_OK) {
        return;
    }
    ComPtr<IStream> writer;
    CreateStreamOverRandomAccessStream(s.Get(), IID_PPV_ARGS(writer.GetAddressOf()));
    writer->Write(content, size, nullptr);
    CComPtr<Streams::IRandomAccessStreamReferenceStatics> rasrs;
    if ((ret = GetActivationFactory(HStringReference(RuntimeClass_Windows_Storage_Streams_RandomAccessStreamReference).Get(), &rasrs)) != S_OK) {
        return;
    }
    CComPtr<Streams::IRandomAccessStreamReference> stream;
    if ((ret = rasrs->CreateFromStream(s.Get(), &stream)) != S_OK) {
        return;
    }
    updater->put_Thumbnail(stream);
}

void MediaTransControls::loadThumbnailFromUrl(CString url) {
    if (url.IsEmpty() || !updater) return;
    if (PathUtils::IsFile(url)) return loadThumbnail(url);
    HRESULT ret;
    CComPtr<ABI::Windows::Foundation::IUriRuntimeClassFactory> u;
    if ((ret = Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_Uri).Get(), &u)) != S_OK) {
        return;
    }
    CComPtr<ABI::Windows::Foundation::IUriRuntimeClass> uri;
    if ((ret = u->CreateUri(HStringReference(url).Get(), &uri)) != S_OK) {
        return;
    }
    CComPtr<Streams::IRandomAccessStreamReferenceStatics> rasrs;
    if ((ret = GetActivationFactory(HStringReference(RuntimeClass_Windows_Storage_Streams_RandomAccessStreamReference).Get(), &rasrs)) != S_OK) {
        return;
    }
    CComPtr<Streams::IRandomAccessStreamReference> stream;
    if ((ret = rasrs->CreateFromUri(uri, &stream)) != S_OK) {
        return;
    }
    ret = updater->put_Thumbnail(stream);
    ASSERT(ret == S_OK);
}
