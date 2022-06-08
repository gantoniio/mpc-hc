/*
 * (C) 2003-2006 Gabest
 * (C) 2006-2016 see Authors.txt
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
#include "mplayerc.h"
#include <cmath>
#include <atlbase.h>
#include <afxpriv.h>
#include "MPCPngImage.h"
#include "PlayerToolBar.h"
#include "MainFrm.h"
#include "PathUtils.h"
#include "SVGImage.h"
#include "ImageGrayer.h"
#include "CMPCTheme.h"
#include "Gdiplusimaging.h"

// CPlayerToolBar

static std::map<int,int> supportedButtons = {
    {ID_PLAY_PLAY, 0},
    {ID_PLAY_PAUSE, 1},
    {ID_PLAY_STOP, 2},
    {ID_NAVIGATE_SKIPBACK, 4},
    {ID_PLAY_DECRATE, 5},
    {ID_PLAY_INCRATE, 6},
    {ID_NAVIGATE_SKIPFORWARD, 7},
    {ID_PLAY_FRAMESTEP, 9},
    {ID_DUMMYSEPARATOR, 11},
    {ID_VOLUME_MUTE, 12},
};

static UINT styles[] = {
    TBBS_CHECKGROUP, TBBS_CHECKGROUP, TBBS_CHECKGROUP,
    TBBS_SEPARATOR,
    TBBS_BUTTON, TBBS_BUTTON, TBBS_BUTTON, TBBS_BUTTON,
    TBBS_SEPARATOR,
    TBBS_BUTTON,
    TBBS_SEPARATOR,
    TBBS_SEPARATOR,
    TBBS_CHECKBOX,
};


IMPLEMENT_DYNAMIC(CPlayerToolBar, CToolBar)
CPlayerToolBar::CPlayerToolBar(CMainFrame* pMainFrame)
    : m_pMainFrame(pMainFrame)
    , m_nButtonHeight(16)
    , m_volumeMinSizeInc(0)
    , mouseDown(false)
    , volumeButtonIndex(12)
    , dummyButtonIndex(11)
    , flexibleSpaceIndex(10)
    , useFlexibleSpace(true)
{
    GetEventd().Connect(m_eventc, {
        MpcEvent::DPI_CHANGED,
        MpcEvent::DEFAULT_TOOLBAR_SIZE_CHANGED,
    }, std::bind(&CPlayerToolBar::EventCallback, this, std::placeholders::_1));
}

CPlayerToolBar::~CPlayerToolBar()
{
}

bool CPlayerToolBar::LoadExternalToolBar(CImage& image, float svgscale)
{
    // Paths and extensions to try (by order of preference)
    std::vector<CString> paths({ PathUtils::GetProgramPath() });
    CString appDataPath;
    if (AfxGetMyApp()->GetAppDataPath(appDataPath)) {
        paths.emplace_back(appDataPath);
    }
    CString basetbname;
    basetbname = _T("buttons.");

    // Try loading the external toolbar
    for (const auto& path : paths) {
        if (SUCCEEDED(SVGImage::Load(PathUtils::CombinePaths(path, basetbname + _T("svg")), image, svgscale))) {
            return true;
        }
    }

    return false;
}

void CPlayerToolBar::LoadToolbarImage()
{
    // We are currently not aware of any cases where the scale factors are different
    float dpiScaling = (float)std::min(m_pMainFrame->m_dpi.ScaleFactorX(), m_pMainFrame->m_dpi.ScaleFactorY());
    int targetsize = int(dpiScaling * AfxGetAppSettings().nDefaultToolbarSize);
    float svgscale = targetsize / 16.0f;

    CImage image;

    m_pButtonsImages.reset();
    m_pDisabledButtonsImages.reset();

    bool buttonsImageLoaded = false;
    if (LoadExternalToolBar(image, svgscale) && image.GetHeight() % 4 == 0 && image.GetWidth() % (image.GetHeight()/4) == 0) {
        buttonsImageLoaded = true;
    }

    if (buttonsImageLoaded || SUCCEEDED(SVGImage::Load(IDF_SVG_BUTTONS, image, svgscale))) {
        CImage imageDisabled;
        CBitmap* bmp = CBitmap::FromHandle(image);


        int width = image.GetWidth();
        int height = image.GetHeight() / 4;
        int bpp = image.GetBPP();
        if (width % height == 0) { //todo: dynamically determine which buttons are supported by this toolbar, otherwise show generic buttons?
            // the manual specifies that sizeButton should be sizeImage inflated by (7, 6)
            SetSizes(CSize(height + 7, height + 6), CSize(height, height));

            int volumeIndex = supportedButtons[ID_VOLUME_MUTE] + 3;
            volumeOn.Destroy();
            volumeOff.Destroy();
            volumeOn.Create(height * 2, height, bpp, CImage::createAlphaChannel);
            volumeOff.Create(height * 2, height, bpp, CImage::createAlphaChannel);

            m_pButtonsImages.reset(DEBUG_NEW CImageList());
            m_pButtonsImages->Create(height, height, ILC_COLOR32 | ILC_MASK, 1, 64);
            m_pDisabledButtonsImages.reset(DEBUG_NEW CImageList());
            m_pDisabledButtonsImages->Create(height, height, ILC_COLOR32 | ILC_MASK, 1, 64);
            
            CImage dynamicToolbar, dynamicToolbarDisabled;

            dynamicToolbar.Create(width, height, bpp, CImage::createAlphaChannel);
            dynamicToolbarDisabled.Create(width, height, bpp, CImage::createAlphaChannel);

            CBitmap* pOldTargetBmp = nullptr;
            CBitmap* pOldSourceBmp = nullptr;
            

            CDC targetDC;
            CDC sourceDC;
            CDC* pDC = this->GetDC();
            targetDC.CreateCompatibleDC(pDC);
            sourceDC.CreateCompatibleDC(pDC);

            pOldTargetBmp = targetDC.SelectObject(CBitmap::FromHandle(dynamicToolbar));
            pOldSourceBmp = sourceDC.GetCurrentBitmap();

            int imageOffset = 0, imageDisabledOffset=height;
            if (AppIsThemeLoaded()) {
                imageOffset = height * 2;
                imageDisabledOffset = height * 3;
            }

            sourceDC.SelectObject(bmp);
            targetDC.BitBlt(0, 0, image.GetWidth(), height, &sourceDC, 0, imageOffset, SRCCOPY);

            targetDC.SelectObject(CBitmap::FromHandle(dynamicToolbarDisabled));
            targetDC.BitBlt(0, 0, image.GetWidth(), height, &sourceDC, 0, imageDisabledOffset, SRCCOPY);

            targetDC.SelectObject(CBitmap::FromHandle(volumeOn));
            targetDC.BitBlt(0, 0, height * 2, height, &sourceDC, volumeIndex * height, imageOffset, SRCCOPY);
            targetDC.SelectObject(CBitmap::FromHandle(volumeOff));
            targetDC.BitBlt(0, 0, height * 2, height, &sourceDC, volumeIndex * height, imageDisabledOffset, SRCCOPY);

            //volumeOn.Save(L"c:\\temp\\vON.png", Gdiplus::ImageFormatPNG);
            //volumeOff.Save(L"c:\\temp\\vOFF.png", Gdiplus::ImageFormatPNG);

            ImageGrayer::PreMultiplyAlpha(volumeOn);
            ImageGrayer::PreMultiplyAlpha(volumeOff);

            sourceDC.SelectObject(pOldSourceBmp);
            targetDC.SelectObject(pOldTargetBmp);

            sourceDC.DeleteDC();
            targetDC.DeleteDC();

            ReleaseDC(pDC);

            m_pButtonsImages->Add(CBitmap::FromHandle(dynamicToolbar), nullptr);
            dynamicToolbar.Destroy();

            m_pDisabledButtonsImages->Add(CBitmap::FromHandle(dynamicToolbarDisabled), nullptr);
            dynamicToolbarDisabled.Destroy();

            m_nButtonHeight = height;

            GetToolBarCtrl().SetImageList(m_pButtonsImages.get());
            GetToolBarCtrl().SetDisabledImageList(m_pDisabledButtonsImages.get());
        }
    }
    image.Destroy();
    
}

BOOL CPlayerToolBar::Create(CWnd* pParentWnd)
{
    VERIFY(__super::CreateEx(pParentWnd,
                             TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_AUTOSIZE | TBSTYLE_CUSTOMERASE,
                             WS_CHILD | WS_VISIBLE | CBRS_BOTTOM /*| CBRS_TOOLTIPS*/,
                             CRect(2, 2, 0, 1)));

    auto& s = AfxGetAppSettings();

    CToolBarCtrl& tb = GetToolBarCtrl();

    dummyButtonIndex = -1;
    volumeButtonIndex = -1;
    flexibleSpaceIndex = -1;
    useFlexibleSpace = false; //can be enabled with right conditions: ID_DUMMYSEPARATOR placed directly before ID_VOLUME_MUTE
    buttonCount = 0;
    sepCount = 0;

    auto addButton = [&](int cmdid, int svgIndex) {
        TBBUTTON button = { 0 };
        button.iBitmap = svgIndex;
        button.idCommand = cmdid;
        button.iString = -1;
        tb.AddButtons(1, &button);
        SetButtonStyle(tb.GetButtonCount() - 1, styles[svgIndex] | TBBS_DISABLED);
        if (cmdid == ID_VOLUME_MUTE) {
            volumeButtonIndex = tb.GetButtonCount() - 1;
            if (dummyButtonIndex == volumeButtonIndex -1) {
                flexibleSpaceIndex = dummyButtonIndex - 1;
                useFlexibleSpace = true;
            }
        } else if (cmdid == ID_DUMMYSEPARATOR) {
            dummyButtonIndex = tb.GetButtonCount() - 1;
        }
        buttonCount++;
    };

    auto addSeparator = [&]() {
        TBBUTTON button = { 0 };
        button.iBitmap = -1;
        button.iString = -1;
        button.fsStyle = BTNS_SEP;
        tb.AddButtons(1, &button);
        sepCount++;
    };

    for (std::vector<int>::size_type i = 0; i < s.toolBarLayout.buttons.size(); ++i) {
        int cmdid = s.toolBarLayout.buttons[i];
        if (cmdid == ID_BUTTONSEP) {
            addSeparator();
        } else if (!useFlexibleSpace) {
            if (supportedButtons.count(cmdid)) {
                int svgIndex = supportedButtons[cmdid];
                addButton(cmdid, svgIndex);
            }
        }
    }

    // Should never be RTLed
    ModifyStyleEx(WS_EX_LAYOUTRTL, WS_EX_NOINHERITLAYOUT);

    SetMute(AfxGetAppSettings().fMute);

    m_volctrl.Create(this);
    m_volctrl.SetRange(0, 100);

    m_nButtonHeight = 16; // reset m_nButtonHeight

    LoadToolbarImage();

    if (AppIsThemeLoaded()) {
        themedToolTip.enableFlickerHelper(); //avoid flicker on button hover
        themedToolTip.Create(this, TTS_ALWAYSTIP);
        tb.SetToolTips(&themedToolTip);
    } else {
        tb.EnableToolTips();
    }

    return TRUE;
}

void CPlayerToolBar::ArrangeControls() {
    if (!::IsWindow(m_volctrl.m_hWnd)) {
        return;
    }

    CRect r;
    GetClientRect(&r);

    CRect br = GetBorders();
    int imageWidth = volumeOn.GetWidth(), imageHeight=volumeOn.GetHeight();
    int volSpace = MulDiv(imageWidth, 12, 10);
    int top = r.top + (r.Height() - imageHeight) / 2;
    CRect vr(r.right + br.right - volSpace, top, r.right + br.right, top+imageHeight);

    m_volctrl.MoveWindow(vr);
    m_volumeMinSizeInc = volSpace;

    if (useFlexibleSpace) {
        UINT nID;
        UINT nStyle;
        int iImage;

        CRect rFlexible;
        GetItemRect(flexibleSpaceIndex, &rFlexible);

        GetButtonInfo(volumeButtonIndex, nID, nStyle, iImage);
        SetButtonInfo(dummyButtonIndex, GetItemID(dummyButtonIndex), TBBS_SEPARATOR, vr.left - iImage - rFlexible.right - (rFlexible.bottom - rFlexible.top) + 11);
    }
}

void CPlayerToolBar::SetMute(bool fMute) {
    CToolBarCtrl& tb = GetToolBarCtrl();
    TBBUTTONINFO bi = { sizeof(bi) };
    if (-1 != tb.GetButtonInfo(ID_VOLUME_MUTE, &bi)) {//volume icon may not appear if dynamic toolbar
        bi.dwMask = TBIF_IMAGE;
        bi.iImage = fMute ? volumeButtonIndex+1 : volumeButtonIndex;
        tb.SetButtonInfo(ID_VOLUME_MUTE, &bi);

    }
    AfxGetAppSettings().fMute = fMute;
}

bool CPlayerToolBar::IsMuted() const
{
    CToolBarCtrl& tb = GetToolBarCtrl();
    TBBUTTONINFO bi = { sizeof(bi) };
    if (-1 != tb.GetButtonInfo(ID_VOLUME_MUTE, &bi)) {//volume icon may not appear if dynamic toolbar
        bi.dwMask = TBIF_IMAGE;
        tb.GetButtonInfo(ID_VOLUME_MUTE, &bi);
        return (bi.iImage == volumeButtonIndex+1);
    }
    return AfxGetAppSettings().fMute;
}

int CPlayerToolBar::GetVolume() const
{
    int volume = m_volctrl.GetPos(); // [0..100]
    if (IsMuted() || volume <= 0) {
        volume = -10000;
    } else {
        volume = std::min((int)(4000 * log10(volume / 100.0f)), 0); // 4000=2.0*100*20, where 2.0 is a special factor
    }

    return volume;
}

int CPlayerToolBar::GetMinWidth() const
{
    return (m_nButtonHeight + 1 + 7) * buttonCount + (1 + 7) * sepCount + m_volumeMinSizeInc;
}

void CPlayerToolBar::SetVolume(int volume)
{
    m_volctrl.SetPosInternal(volume);
}

void CPlayerToolBar::EventCallback(MpcEvent ev)
{
    switch (ev) {
        case MpcEvent::DPI_CHANGED:
        case MpcEvent::DEFAULT_TOOLBAR_SIZE_CHANGED:
            LoadToolbarImage();
            break;
        default:
            UNREACHABLE_CODE();
    }
}

BEGIN_MESSAGE_MAP(CPlayerToolBar, CToolBar)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
    ON_WM_SIZE()
    ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)
    ON_COMMAND_EX(ID_VOLUME_MUTE, OnVolumeMute)
    ON_UPDATE_COMMAND_UI(ID_VOLUME_MUTE, OnUpdateVolumeMute)
    ON_COMMAND_EX(ID_VOLUME_UP, OnVolumeUp)
    ON_COMMAND_EX(ID_VOLUME_DOWN, OnVolumeDown)
    ON_WM_NCPAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CPlayerToolBar message handlers

void drawButtonBG(NMCUSTOMDRAW nmcd, COLORREF c)
{
    CDC dc;
    dc.Attach(nmcd.hdc);
    CRect br;
    br.CopyRect(&nmcd.rc);
    br.DeflateRect(0, 0, 1, 1); //we aren't offsetting button when pressed, so try to center better

    dc.FillSolidRect(br, c);

    CBrush fb;
    fb.CreateSolidBrush(CMPCTheme::PlayerButtonBorderColor);
    dc.FrameRect(br, &fb);
    fb.DeleteObject();
    dc.Detach();
}

void CPlayerToolBar::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTBCUSTOMDRAW pTBCD = reinterpret_cast<LPNMTBCUSTOMDRAW>(pNMHDR);
    LRESULT lr = CDRF_DODEFAULT;

    switch (pTBCD->nmcd.dwDrawStage) {
        case CDDS_PREERASE:
            m_volctrl.Invalidate();
            lr = CDRF_DODEFAULT;
            break;
        case CDDS_PREPAINT: {
            // paint the control background, this is needed for XP
            CDC dc;
            dc.Attach(pTBCD->nmcd.hdc);
            RECT r;
            GetClientRect(&r);
            if (AppIsThemeLoaded()) {
                dc.FillSolidRect(&r, CMPCTheme::PlayerBGColor);
            } else {
                dc.FillSolidRect(&r, ::GetSysColor(COLOR_BTNFACE));
            }
            dc.Detach();
        }
        lr |= CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
        break;
        case CDDS_ITEMPREPAINT:
            lr |= CDRF_NOTIFYPOSTPAINT | TBCDRF_NOOFFSET;
            {
                if (AppIsThemeLoaded()) {
                    lr |= TBCDRF_NOBACKGROUND;
                    if (pTBCD->nmcd.uItemState & CDIS_CHECKED) {
                        drawButtonBG(pTBCD->nmcd, CMPCTheme::PlayerButtonCheckedColor);
                    } else if (pTBCD->nmcd.uItemState & CDIS_HOT) {
                        drawButtonBG(pTBCD->nmcd, mouseDown ? CMPCTheme::PlayerButtonClickedColor : CMPCTheme::PlayerButtonHotColor);
                    }
                }
            }
            break;
        case CDDS_ITEMPOSTPAINT:
            // paint over the duplicated separator
            CDC dc;
            dc.Attach(pTBCD->nmcd.hdc);
            RECT r;
            GetItemRect(dummyButtonIndex, &r);
            if (AppIsThemeLoaded()) {
                dc.FillSolidRect(&r, CMPCTheme::PlayerBGColor);
            } else {
                dc.FillSolidRect(&r, GetSysColor(COLOR_BTNFACE));
            }
            dc.Detach();
            lr |= CDRF_SKIPDEFAULT;
            break;
    }

    *pResult = lr;
}

void CPlayerToolBar::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);

    ArrangeControls();
}

void CPlayerToolBar::OnInitialUpdate()
{
    ArrangeControls();
}

BOOL CPlayerToolBar::OnVolumeMute(UINT nID)
{
    SetMute(!IsMuted());
    return FALSE;
}

void CPlayerToolBar::OnUpdateVolumeMute(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(true);
    pCmdUI->SetCheck(IsMuted());
}

BOOL CPlayerToolBar::OnVolumeUp(UINT nID)
{
    m_volctrl.IncreaseVolume();
    return FALSE;
}

BOOL CPlayerToolBar::OnVolumeDown(UINT nID)
{
    m_volctrl.DecreaseVolume();
    return FALSE;
}

void CPlayerToolBar::OnNcPaint() // when using XP styles the NC area isn't drawn for our toolbar...
{
    CRect wr, cr;

    CWindowDC dc(this);
    GetClientRect(&cr);
    ClientToScreen(&cr);
    GetWindowRect(&wr);
    cr.OffsetRect(-wr.left, -wr.top);
    wr.OffsetRect(-wr.left, -wr.top);
    dc.ExcludeClipRect(&cr);

    if (AppIsThemeLoaded()) {
        dc.FillSolidRect(wr, CMPCTheme::PlayerBGColor);
    } else {
        dc.FillSolidRect(wr, ::GetSysColor(COLOR_BTNFACE));
    }

    // Do not call CToolBar::OnNcPaint() for painting messages

    // Invalidate window to force repaint the expanded separator
    Invalidate(FALSE);
}

BOOL CPlayerToolBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    BOOL ret = FALSE;
    if (nHitTest == HTCLIENT) {
        CPoint point;
        VERIFY(GetCursorPos(&point));
        ScreenToClient(&point);

        int i = getHitButtonIdx(point);
        if (i >= 0 && !(GetButtonStyle(i) & (TBBS_SEPARATOR | TBBS_DISABLED))) {
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
            ret = TRUE;
        }
    }
    return ret ? ret : __super::OnSetCursor(pWnd, nHitTest, message);
}

void CPlayerToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    int i = getHitButtonIdx(point);
    mouseDown = true;

    if (!m_pMainFrame->m_fFullScreen && (i < 0 || (GetButtonStyle(i) & (TBBS_SEPARATOR | TBBS_DISABLED)))) {
        ClientToScreen(&point);
        m_pMainFrame->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
    } else {
        __super::OnLButtonDown(nFlags, point);
    }
}

int CPlayerToolBar::getHitButtonIdx(CPoint point)
{
    int hit = -1; // -1 means not on any buttons, mute button is 12/13, others < 10, 11 is empty space between (adipose: dynamic toolbar positions may differ)
    CRect r;

    for (int i = 0, j = GetToolBarCtrl().GetButtonCount(); i < j; i++) {
        GetItemRect(i, r);

        if (r.PtInRect(point)) {
            hit = i;
            break;
        }
    }

    return hit;
}

BOOL CPlayerToolBar::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
    TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;

    int nID;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)pNMHDR->idFrom);
    } else {
        nID = (int)pNMHDR->idFrom;
    }
    const auto& s = AfxGetAppSettings();
    auto& buttonIDs = s.toolBarLayout.buttons;
    if (nID != ID_VOLUME_MUTE && std::find(buttonIDs.begin(),buttonIDs.end(),nID) == buttonIDs.end()) {
        return FALSE;
    }
    CToolBarCtrl& tb = GetToolBarCtrl();

    TBBUTTONINFO bi;
    bi.cbSize = sizeof(bi);
    bi.dwMask = TBIF_IMAGE;
    if (-1 == tb.GetButtonInfo(nID, &bi)) { //should only fail if they choose not to show volume button
        return FALSE;
    }

    static CString strTipText;
    if (nID != ID_VOLUME_MUTE) { 
        if (s.CommandIDToWMCMD.count(nID) > 0) {
            strTipText.LoadString(s.CommandIDToWMCMD[nID]->dwname);
        } else {
            return FALSE;
        }
    } else if (bi.iImage == volumeButtonIndex) {
        strTipText.LoadString(ID_VOLUME_MUTE);
    } else if (bi.iImage == volumeButtonIndex+1) {
        strTipText.LoadString(ID_VOLUME_MUTE_OFF);
    } else if (bi.iImage == volumeButtonIndex+2) {
        strTipText.LoadString(ID_VOLUME_MUTE_DISABLED);
    } else {
        return FALSE;
    }
    pTTT->lpszText = (LPWSTR)(LPCWSTR)strTipText;

    *pResult = 0;


    return TRUE;    // message was handled
}

void CPlayerToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    mouseDown = false;
    CToolBar::OnLButtonUp(nFlags, point);
}
