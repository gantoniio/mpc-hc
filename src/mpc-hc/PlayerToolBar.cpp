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

// CPlayerToolBar

#define USE_LEGACY_TOOLBAR 0

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

bool CPlayerToolBar::LoadExternalToolBar(CImage& image, bool useColor, bool dynamicIcons /*=false*/)
{
    // Paths and extensions to try (by order of preference)
    std::vector<CString> paths({ PathUtils::GetProgramPath() });
    CString appDataPath;
    if (AfxGetMyApp()->GetAppDataPath(appDataPath)) {
        paths.emplace_back(appDataPath);
    }
    CString basetbname;
    if (AppIsThemeLoaded()) {
        basetbname = _T("toolbar_dark.");
    } else {
        basetbname = _T("toolbar.");
    }

    if (useColor) {
        basetbname = _T("color_") + basetbname;
    }

    if (dynamicIcons) {
        basetbname = _T("dynamic_") + basetbname;
    }

    // TODO: Find a better solution?
    float dpiScaling = (float)std::min(m_pMainFrame->m_dpi.ScaleFactorX(), m_pMainFrame->m_dpi.ScaleFactorY());

    // Try loading the external toolbar
    for (const auto& path : paths) {
        if (SUCCEEDED(SVGImage::Load(PathUtils::CombinePaths(path, basetbname + _T("svg")), image, dpiScaling))) {
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

    CImage image, themedImage, origImage;
    CImage dynamicButtonsImage, dynamicThemedButtonsImage, dynamicOrigImage;
    m_pButtonsImages.reset();
    m_pDisabledButtonsImages.reset();

    bool colorToolbar = false, toolbarImageLoaded = false, dynamicButtonsImageLoaded=false;
    if (LoadExternalToolBar(origImage, true)) {
        colorToolbar = true;
        toolbarImageLoaded = true;
    } else if (LoadExternalToolBar(origImage, false)) {
        toolbarImageLoaded = true;
    }

    if (toolbarImageLoaded || SUCCEEDED(SVGImage::Load(IDF_SVG_TOOLBAR, origImage, svgscale))) {
        if (0 == USE_LEGACY_TOOLBAR && LoadExternalToolBar(dynamicOrigImage, colorToolbar, true)) {
            if (dynamicOrigImage.GetHeight() == origImage.GetHeight() && 0 == dynamicOrigImage.GetWidth() % origImage.GetHeight()) {
                dynamicButtonsImageLoaded = true;
            } else {
                dynamicButtonsImageLoaded = false;
            }
        }

        if (AppIsThemeLoaded() && colorToolbar == false) {
            ImageGrayer::UpdateColor(origImage, themedImage, false, ImageGrayer::mpcMono);
            image = themedImage;
            if (dynamicButtonsImageLoaded) {
                ImageGrayer::UpdateColor(dynamicOrigImage, dynamicThemedButtonsImage, false, ImageGrayer::mpcMono);
                dynamicButtonsImage = dynamicThemedButtonsImage;
            }
        } else {
            image = origImage;
            dynamicButtonsImage = dynamicOrigImage;
        }
        CImage imageDisabled;
        bool createdDisabledImage = false;
        CBitmap* bmp = CBitmap::FromHandle(image);
        int width = image.GetWidth();
        int height = image.GetHeight();
        int bpp = image.GetBPP();
        if (width == height * 15) {
            // the manual specifies that sizeButton should be sizeImage inflated by (7, 6)
            SetSizes(CSize(height + 7, height + 6), CSize(height, height));

            m_pButtonsImages.reset(DEBUG_NEW CImageList());
            if (bpp == 32) {
                m_pButtonsImages->Create(height, height, ILC_COLOR32 | ILC_MASK, 1, 0);
            } else {
                m_pButtonsImages->Create(height, height, ILC_COLOR24 | ILC_MASK, 1, 0);
            }

            if (USE_LEGACY_TOOLBAR) {
                if (bpp == 32) {
                    m_pButtonsImages->Add(bmp, nullptr); // alpha is the mask
                } else {
                    m_pButtonsImages->Add(bmp, RGB(255, 0, 255));
                }
                if (colorToolbar == false && bpp == 32) {//if color toolbar, we assume the imagelist can gray itself nicely, rather than using imagegrayer
                    createdDisabledImage = ImageGrayer::UpdateColor(origImage, imageDisabled, true, AppIsThemeLoaded() ? ImageGrayer::mpcMono : ImageGrayer::classicGrayscale);
                }
            } else { //dynamic toolbar
                CImage dynamicToolbar;
                int dynamicWidth = width;
                if (dynamicButtonsImageLoaded) {
                    dynamicWidth += dynamicButtonsImage.GetWidth();
                }
                dynamicToolbar.Create(dynamicWidth, height, bpp, 32 == bpp ? CImage::createAlphaChannel : 0);

                CBitmap* pOldTargetBmp = nullptr;
                CBitmap* pOldSourceBmp = nullptr;

                CDC targetDC;
                CDC sourceDC;
                CDC* pDC = this->GetDC();
                targetDC.CreateCompatibleDC(pDC);
                sourceDC.CreateCompatibleDC(pDC);
                pOldTargetBmp = targetDC.SelectObject(CBitmap::FromHandle(dynamicToolbar));
                pOldSourceBmp = sourceDC.GetCurrentBitmap();

                sourceDC.SelectObject(CBitmap::FromHandle(image));
                targetDC.BitBlt(0, 0, image.GetWidth(), height, &sourceDC, 0, 0, SRCCOPY);

                if (dynamicButtonsImageLoaded) {
                    sourceDC.SelectObject(CBitmap::FromHandle(dynamicButtonsImage));
                    targetDC.BitBlt(image.GetWidth(), 0, dynamicButtonsImage.GetWidth(), height, &sourceDC, 0, 0, SRCCOPY);
                }

                sourceDC.SelectObject(pOldSourceBmp);
                targetDC.SelectObject(pOldTargetBmp);

                sourceDC.DeleteDC();
                targetDC.DeleteDC();

                ReleaseDC(pDC);

                if (32 == bpp) {
                    m_pButtonsImages->Add(CBitmap::FromHandle(dynamicToolbar), nullptr);
                } else {
                    m_pButtonsImages->Add(CBitmap::FromHandle(dynamicToolbar), RGB(255, 0, 255));
                }
                dynamicToolbar.Save(_T("c:\\temp\\toolbar.bmp"));
                if (colorToolbar == false && bpp == 32) {//if color toolbar, we assume the imagelist can gray itself nicely, rather than using imagegrayer
                    createdDisabledImage = ImageGrayer::UpdateColor(dynamicToolbar, imageDisabled, true, AppIsThemeLoaded() ? ImageGrayer::mpcMono : ImageGrayer::classicGrayscale);
                }
                dynamicToolbar.Destroy();
            }
            if (createdDisabledImage) {
                m_pDisabledButtonsImages.reset(DEBUG_NEW CImageList());
                m_pDisabledButtonsImages->Create(height, height, ILC_COLOR32 | ILC_MASK, 1, 0);
                m_pDisabledButtonsImages->Add(CBitmap::FromHandle(imageDisabled), nullptr); // alpha is the mask
                imageDisabled.Destroy();
            } else {
                m_pDisabledButtonsImages = nullptr;
            }

            m_nButtonHeight = height;

            GetToolBarCtrl().SetImageList(m_pButtonsImages.get());
            GetToolBarCtrl().SetDisabledImageList(m_pDisabledButtonsImages.get());
        }
        if (themedImage) {
            themedImage.Destroy();
        }
        if (dynamicThemedButtonsImage) {
            dynamicThemedButtonsImage.Destroy();
        }
    }
    origImage.Destroy();
    if (dynamicOrigImage) {
        dynamicOrigImage.Destroy();
    }
    
}

BOOL CPlayerToolBar::Create(CWnd* pParentWnd)
{
    VERIFY(__super::CreateEx(pParentWnd,
                             TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_AUTOSIZE | TBSTYLE_CUSTOMERASE,
                             WS_CHILD | WS_VISIBLE | CBRS_BOTTOM /*| CBRS_TOOLTIPS*/,
                             CRect(2, 2, 0, 1)));

    auto& s = AfxGetAppSettings();

    CToolBarCtrl& tb = GetToolBarCtrl();
    UINT styles[] = {
        TBBS_CHECKGROUP, TBBS_CHECKGROUP, TBBS_CHECKGROUP,
        TBBS_SEPARATOR,
        TBBS_BUTTON, TBBS_BUTTON, TBBS_BUTTON, TBBS_BUTTON,
        TBBS_SEPARATOR,
        TBBS_BUTTON,
        TBBS_SEPARATOR,
        TBBS_SEPARATOR,
        TBBS_CHECKBOX,
    };

    if (USE_LEGACY_TOOLBAR) {
        VERIFY(LoadToolBar(IDB_PLAYERTOOLBAR));
        tb.DeleteButton(tb.GetButtonCount() - 1);
        tb.DeleteButton(tb.GetButtonCount() - 1);
        
        for (int i = 0; i < _countof(styles); ++i) {
            // This fixes missing separator in Win 7
            if (styles[i] & TBBS_SEPARATOR) {
                SetButtonInfo(i, GetItemID(i), styles[i], -1);
            } else {
                SetButtonStyle(i, styles[i] | TBBS_DISABLED);
            }
        }
    } else {
        CToolBar legacyTB;
        legacyTB.CreateEx(pParentWnd, TBSTYLE_FLAT, WS_CHILD, CRect(2, 2, 0, 1));
        VERIFY(legacyTB.LoadToolBar(IDB_PLAYERTOOLBAR));
        auto& ltbCtrl = legacyTB.GetToolBarCtrl();
        ASSERT(ltbCtrl.GetButtonCount() == 15);

        idToLegacyButtonIndex.clear();
        dummyButtonIndex = -1;
        volumeButtonIndex = -1;
        flexibleSpaceIndex = -1;
        useFlexibleSpace = false; //can be enabled with right conditions: ID_DUMMYSEPARATOR placed directly before ID_VOLUME_MUTE

        for (int i = 0; i < ltbCtrl.GetButtonCount(); i++) {
            TBBUTTON button;
            ltbCtrl.GetButton(i, &button);
            idToLegacyButtonIndex[button.idCommand] = i;
        }

        auto addLegacyButton = [&](int cmdid) {
            int legacyButtonIndex = idToLegacyButtonIndex[cmdid];
            TBBUTTON lb;
            ltbCtrl.GetButton(legacyButtonIndex, &lb);

            tb.AddButtons(1, &lb);
            SetButtonStyle(tb.GetButtonCount() - 1, styles[legacyButtonIndex] | TBBS_DISABLED);
            if (cmdid == ID_VOLUME_MUTE) {
                volumeButtonIndex = tb.GetButtonCount() - 1;
                if (dummyButtonIndex == volumeButtonIndex -1) {
                    flexibleSpaceIndex = dummyButtonIndex - 1;
                    useFlexibleSpace = true;
                }
            } else if (cmdid == ID_DUMMYSEPARATOR) {
                dummyButtonIndex = tb.GetButtonCount() - 1;
            }
        };

        auto addSeparator = [&]() {
            TBBUTTON button = { 0 };
            button.iBitmap = -1;
            button.iString = -1;
            button.fsStyle = BTNS_SEP;
            tb.AddButtons(1, &button);
        };

        auto addDynamicButton = [&](int cmdid, int index) {
            TBBUTTON button = { 0 };
            button.iBitmap = index;
            button.idCommand = cmdid;
            button.iString = -1;
            button.fsStyle = BTNS_BUTTON;
            tb.AddButtons(1, &button);
            SetButtonStyle(tb.GetButtonCount()-1, TBBS_BUTTON | TBBS_DISABLED);
        };

        for (std::vector<int>::size_type i = 0; i < s.toolBarLayout.buttons.size(); ++i) {
            int cmdid = s.toolBarLayout.buttons[i];
            int svgIndex = s.toolBarLayout.SVGIndex[i];
            if (cmdid == ID_BUTTONSEP) {
                addSeparator();
            } else if (cmdid == ID_DUMMYSEPARATOR || (idToLegacyButtonIndex.count(cmdid) > 0 && svgIndex == -1)) { //is a legacy button included in toolbar.svg, or is special case dummyseparator
                addLegacyButton(cmdid);
            } else if (!useFlexibleSpace) { //any command not included in legacy toolbar.svg. if we've already passed the spacer for volume, ignore it as there's nowhere to put it
                addDynamicButton(cmdid, svgIndex);
            }
            int xxx = tb.GetButtonCount();
            xxx = 1;
        }

        ltbCtrl.DestroyWindow();
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

    CRect vr(r.right + br.right - 60, r.top - 2, r.right + br.right + 6, r.bottom);
    m_volctrl.MoveWindow(vr);

    CRect thumbRect;
    m_volctrl.GetThumbRect(thumbRect);
    m_volctrl.MapWindowPoints(this, thumbRect);
    vr.top += std::max((r.bottom - thumbRect.bottom - 4) / 2, 0l);
    vr.left -= m_volumeMinSizeInc = MulDiv(thumbRect.Height(), 50, 19) - 50;
    m_volctrl.MoveWindow(vr);

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
    return m_nButtonHeight * 9 + 155 + m_volumeMinSizeInc;
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
            lr |= CDRF_NOTIFYPOSTPAINT;
            {
                if (AppIsThemeLoaded()) {
                    lr |= TBCDRF_NOBACKGROUND | TBCDRF_NOOFFSET;
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
        MapWindowPoints(m_pMainFrame, &point, 1);
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
    if (nID != ID_VOLUME_MUTE) { //handle any buttons that don't exist on legacy toolbar
        if (idToLegacyButtonIndex.count(nID) == 0 && s.CommandIDToWMCMD.count(nID) > 0) {
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
