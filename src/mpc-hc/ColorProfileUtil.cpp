#include "stdafx.h"
#include "ColorProfileUtil.h"
#include "lcms2/library/include/lcms2.h"
#include "MainFrm.h"
#include "Icm.h"

HANDLE ColorProfileUtil::listenThreadHandle = 0;
HANDLE ColorProfileUtil::quitEvent = 0;
CWnd* ColorProfileUtil::mainFrame = nullptr;

TCHAR* ColorProfileUtil::getIccProfilePath(HWND wnd) {
	TCHAR* iccProfilePath = 0;

	HMONITOR hMonitor = MonitorFromWindow(wnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);

	HDC hDC = CreateDC(_T("DISPLAY"), miex.szDevice, nullptr, nullptr);

	if (hDC != nullptr) {
		DWORD icmProfilePathSize = 0;
		GetICMProfile(hDC, &icmProfilePathSize, nullptr);
		iccProfilePath = DEBUG_NEW TCHAR[icmProfilePathSize];
		if (!GetICMProfile(hDC, &icmProfilePathSize, iccProfilePath)) {
			delete[] iccProfilePath;
			iccProfilePath = 0;
		}
		//hack for realtime changes--GetICMProfile requires process restart, even though it accesses reg keys again (verified with procmon)
		for (int hc = 0; hc < 2; hc++) {
			DWORD dwKeySize, dwKeyType;
			TCHAR iccFile[1024];
			dwKeySize = sizeof(iccFile);
			LONG lResult = RegGetValue(hc == 0 ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE, REGISTERED_PROFILES_KEY, REGISTERED_PROFILES_SRGB,
				RRF_RT_REG_SZ, &dwKeyType, (PVOID)&iccFile, &dwKeySize);
			if (lResult == ERROR_SUCCESS) {
				TCHAR* tmpProfilePath = DEBUG_NEW TCHAR[icmProfilePathSize + dwKeySize];
				tmpProfilePath[0] = 0;
				TCHAR* lastSlash = _tcsrchr(iccProfilePath, _T('\\'));
				if (lastSlash) {
					*(lastSlash + 1) = 0; //terminate after slash
				}
				_tcscat_s(tmpProfilePath, icmProfilePathSize + dwKeySize, iccProfilePath);
				_tcscat_s(tmpProfilePath, icmProfilePathSize + dwKeySize, iccFile);
				delete[] iccProfilePath;
				iccProfilePath = tmpProfilePath;
				break; //quit if we found a match in HKCU, otherwise loop to check for HKLM
			}
		}

		DeleteDC(hDC);
	}
	return iccProfilePath;
}

bool ColorProfileUtil::applyColorProfile(HWND wnd, CImage &image) {
    cmsHPROFILE hInputProfile = cmsCreate_sRGBProfile();

    cmsHPROFILE hOutputProfile=nullptr;
    FILE* outputProfileStream = nullptr;

	TCHAR* iccProfilePath = getIccProfilePath(wnd);

    if (iccProfilePath != 0) {
        if (_wfopen_s(&outputProfileStream, T2W(iccProfilePath), L"rb") != 0) {
            cmsCloseProfile(hInputProfile);
            return false;
        }
        hOutputProfile = cmsOpenProfileFromStream(outputProfileStream, "r");
        delete[] iccProfilePath;
    }

	if (!hOutputProfile) {
		hOutputProfile = cmsCreate_sRGBProfile();
	}

    cmsHTRANSFORM hTransform;
    hTransform = cmsCreateTransform(hInputProfile, TYPE_BGR_8, hOutputProfile, TYPE_BGR_8, INTENT_PERCEPTUAL, 0);

    if (hTransform) {
		BYTE* bits = static_cast<BYTE*>(image.GetBits());
		for (int y = 0; y < image.GetHeight(); y++, bits += image.GetPitch()) {
			RGBQUAD* p = reinterpret_cast<RGBQUAD*>(bits);
			cmsDoTransform(hTransform, p, p, image.GetWidth());
		}
		cmsDeleteTransform(hTransform);
	}

    cmsCloseProfile(hInputProfile);
    cmsCloseProfile(hOutputProfile);

	return true;
}

DWORD WINAPI ColorProfileUtil::ListenThread(LPVOID lpParam) {
	SetThreadName(DWORD(-1), "ColorProfileUtil::ListenThread");

	if (!mainFrame) return 1;

	DWORD  dwFilter = REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_ATTRIBUTES | REG_NOTIFY_CHANGE_LAST_SET | REG_NOTIFY_CHANGE_SECURITY;

	HANDLE hEvents[3];
	HKEY   hKeys[3];
	LONG   lErrorCode;


	for (int hc = 0; hc < 2; hc++) {
		RegOpenKeyEx(hc == 0 ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, REGISTERED_PROFILES_KEY, 0, KEY_READ, &hKeys[hc]);
		hEvents[hc] = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		if (hEvents[hc] == nullptr) {
			TRACE(_T("ColorProfileUtil::ListenThread could not create event\n"));
			return 1;
		}

		lErrorCode = RegNotifyChangeKeyValue(hKeys[hc], TRUE, dwFilter, hEvents[hc], TRUE);

		if (lErrorCode != ERROR_SUCCESS) {
			TRACE(_T("ColorProfileUtil::ListenThread failed in RegNotifyChangeKeyValue\n"));
			return 1;
		}
	}

	hEvents[2] = quitEvent;

	while (true) {
		DWORD dwObject = WaitForMultipleObjects(3, hEvents, false, INFINITE);
		if (dwObject == WAIT_FAILED) {
			TRACE(_T("ColorProfileUtil::ListenThread failed in RegNotifyChangeKeyValue\n"));
			return 1;
		} else if (dwObject == WAIT_OBJECT_0+2) {
			break;
		} else if (dwObject == WAIT_OBJECT_0 || dwObject == WAIT_OBJECT_0 + 1) {
			PostMessage(mainFrame->GetSafeHwnd(), WM_COLOR_PROFILE_CHANGED, 0, 0);
			RegNotifyChangeKeyValue(hKeys[dwObject - WAIT_OBJECT_0], TRUE, dwFilter, hEvents[dwObject - WAIT_OBJECT_0], TRUE);
		}
	}

	for (int hc = 0; hc < 2; hc++) {
		lErrorCode = RegCloseKey(hKeys[hc]);
		if (lErrorCode != ERROR_SUCCESS) {
			TRACE(_T("ColorProfileUtil::ListenThread failed to close key\n"));
		}

		if (!CloseHandle(hEvents[hc])) {
			TRACE(_T("ColorProfileUtil::ListenThread failed to close handle\n"));
		}
	}

	return 0;
}


void ColorProfileUtil::listenForProfileChange(CWnd* _mainFrame) {
	DWORD threadId;
	ColorProfileUtil::mainFrame = _mainFrame;
	quitEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	listenThreadHandle = ::CreateThread(nullptr, 0, ListenThread, 0, 0, &threadId);
}

void ColorProfileUtil::stopListeningForProfileChange() {
	if (quitEvent) {
		SetEvent(quitEvent);
	}
	if (listenThreadHandle && WaitForSingleObject(listenThreadHandle, 10000) == WAIT_TIMEOUT) {
		ASSERT(FALSE);
		TerminateThread(listenThreadHandle, 0xDEAD);
	}
}
