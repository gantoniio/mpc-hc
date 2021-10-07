#include "stdafx.h"
#include "MediaTransControls.h"

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

bool MediaTransControls::configure(HWND main) {
    if (!IsWindowsVersionOrLater(0x06030000ul)) return false;
    CComPtr<ISystemMediaTransportControlsInterop> op;
    HRESULT ret;
    if ((ret = GetActivationFactory(HStringReference(L"Windows.Media.SystemMediaTransportControls").Get(), &op)) != S_OK) {
        this->controls = nullptr;
        return false;
    }
    if ((ret = op->GetForWindow(main, IID_PPV_ARGS(&this->controls))) != S_OK) {
        this->controls = nullptr;
        return false;
    }
    this->controls->put_PlaybackStatus(MediaPlaybackStatus::MediaPlaybackStatus_Closed);
    assert(S_OK == this->controls->get_DisplayUpdater(&this->updater));
    assert(S_OK == this->updater->put_Type(MediaPlaybackType::MediaPlaybackType_Video));
    assert(S_OK == this->updater->get_VideoProperties(&this->video));
    return true;
}

void MediaTransControls::stop() {
    if (this->controls != nullptr) {
        this->controls->put_IsEnabled(false);
        this->controls->put_PlaybackStatus(MediaPlaybackStatus::MediaPlaybackStatus_Closed);
        this->updater->ClearAll();
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
    HRESULT ret;
    CComPtr<IStorageFileStatics> sfs;
    if ((ret = GetActivationFactory(HStringReference(L"Windows.Storage.StorageFile").Get(), &sfs)) != S_OK) {
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
    if ((ret = GetActivationFactory(HStringReference(L"Windows.Storage.Streams.RandomAccessStreamReference").Get(), &rasrs)) != S_OK) {
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
    wchar_t pb[MAX_PATH + 1];
    if (!GetTempPathW(MAX_PATH + 1, pb)) {
        return;
    }
    wchar_t pn[MAX_PATH + 1];
    if (!GetTempFileNameW(pb, L"thumbnail_", 0, pn)) return;
    FILE* f;
    if (_wfopen_s(&f, pn, L"wb")) return;
    if (!f) return;
    fwrite(content, sizeof(BYTE), size, f);
    fclose(f);
    loadThumbnail(pn);
}
