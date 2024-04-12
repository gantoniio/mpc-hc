/*
 * (C) 2003-2006 Gabest
 * (C) 2006-2012, 2014-2017 see Authors.txt
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

#pragma once

#include "PPageBase.h"
#include "resource.h"
#include "CMPCThemePPageBase.h"
#include "CMPCThemeComboBox.h"


// CPPageOutput dialog

enum VideoFeatureSupport {
    VIDRND_DXVA_SUPPORT = 0,
    VIDRND_SAVEIMAGE_SUPPORT,
    VIDRND_SHADER_SUPPORT,
    VIDRND_ROTATION_SUPPORT,
    VIDRND_HDR_SUPPORT,
    VIDRND_COUNT
};

typedef std::unordered_map<int, std::set<int>> featureSupportMap;

class CPPageOutput : public CMPCThemePPageBase
{
    DECLARE_DYNAMIC(CPPageOutput)

private:
    CStringArray m_AudioRendererDisplayNames;
    CStringArray m_D3D9GUIDNames;

    CMPCThemeComboBox m_iDSVideoRendererTypeCtrl;
    CMPCThemeComboBox m_iAudioRendererTypeCtrl;
    CMPCThemeComboBox m_SubtitleRendererCtrl;
    CMPCThemeComboBox m_iD3D9RenderDeviceCtrl;
    CMPCThemeComboBox m_APSurfaceUsageCtrl;
    CMPCThemeComboBox m_DX9ResizerCtrl;
    CMPCThemeComboBox m_EVRBuffersCtrl;

    void UpdateSubtitleSupport();

    void UpdateSubtitleRendererList();

public:
    CPPageOutput();
    virtual ~CPPageOutput();

    void UpdateAudioRenderer(CString audioRendererStr);

    // Dialog Data
    enum { IDD = IDD_PPAGEOUTPUT };
    int m_iDSVideoRendererType;
    int m_iAPSurfaceUsage;
    int m_iAudioRendererType;
    int m_iMPCAudioRendererType;
    int m_iSaneAudioRendererType;
    CAppSettings::SubtitleRenderer m_lastSubrenderer;
    int m_iDX9Resizer;
    BOOL m_fVMR9MixerMode;
    BOOL m_fD3DFullscreen;
    BOOL m_fVMR9AlterativeVSync;
    BOOL m_fResetDevice;
    BOOL m_fCacheShaders;
    CString m_iEvrBuffers;
    static featureSupportMap videoFeaturesSupported;
    std::set<int> supportedVideoFeatures;
    bool subtitlesSupported;
    void SetFeatureTooltips();

    BOOL m_fD3D9RenderDevice;
    int m_iD3D9RenderDevice;
    const CString& GetAudioRendererDisplayName();
    CStringW m_vidUnsupportedStr, m_audioUnsupportedStr;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual BOOL OnApply();

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnUpdateVideoRendererSettings(CCmdUI* pCmdUI);
    afx_msg void OpenVideoRendererSettings();
    afx_msg void OnUpdateAudioRendererSettings(CCmdUI* pCmdUI);
    afx_msg void OpenAudioRendererSettings();
    afx_msg void OnSurfaceChange();
    afx_msg void OnDSRendererChange();
    afx_msg void OnAudioRendererChange();
    afx_msg void OnSubtitleRendererChange();
    afx_msg void OnFullscreenCheck();
    afx_msg void OnD3D9DeviceCheck();
};
