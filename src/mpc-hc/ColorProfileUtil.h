#pragma once
#include <algorithm>
namespace Gdiplus { //avoid GDIPLUS min/max issues when including atlimage
	using std::min;
	using std::max;
}
#include <atlimage.h>

#define REGISTERED_PROFILES_KEY TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\ICM\\RegisteredProfiles")
#define REGISTERED_PROFILES_SRGB TEXT("sRGB")


class ColorProfileUtil {
public:
	static TCHAR* getIccProfilePath(HWND wnd);
    static bool applyColorProfile(HWND wnd, CImage &image);
	static void listenForProfileChange(CWnd* mainFrame);
	static void stopListeningForProfileChange();
	static DWORD WINAPI ColorProfileUtil::ListenThread(LPVOID lpParam);
private:
	static HANDLE listenThreadHandle, quitEvent;
	static CWnd* mainFrame;
};

