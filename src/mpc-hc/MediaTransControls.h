#pragma once
#include <sdkddkver.h>

#include <atlbase.h>
#include <windows.media.h>

class MediaTransControls {
public:
    MediaTransControls(void) {
        this->controls = nullptr;
        this->updater = nullptr;
    }
    bool configure(HWND main);
    /**
     * @brief Set status to stoped and clear all metadata infromations.
    */
    void stop();
    /**
     * @brief Change status to play status.
    */
    void play();
    CComPtr<ABI::Windows::Media::ISystemMediaTransportControls> controls;
    CComPtr<ABI::Windows::Media::ISystemMediaTransportControlsDisplayUpdater> updater;
    CComPtr<ABI::Windows::Media::IVideoDisplayProperties> video;
    void loadThumbnail(CString fn);
    void loadThumbnail(BYTE* content, size_t size);
};
