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

//svg has 30 positions
//0-7 are standard mpc buttons
//8-24 reserved for additional toolbar buttons
//25-27 are volume mute variants
//28-29 reserved for volume svg

#define VOLUMEBUTTON_SVG_INDEX 25
#define VOLUME_SVG_INDEX 28

std::map<int, CPlayerToolBar::svgButtonInfo> CPlayerToolBar::supportedSvgButtons = {
    {ID_PLAY_PLAY, {TBBS_CHECKGROUP,0,true}},
    {ID_PLAY_PAUSE, {TBBS_CHECKGROUP,1,true}},
    {ID_PLAY_STOP, {TBBS_CHECKGROUP,2,true}},
    {ID_NAVIGATE_SKIPBACK, {TBBS_BUTTON,3}},
    {ID_PLAY_DECRATE, {TBBS_BUTTON,4}},
    {ID_PLAY_INCRATE, {TBBS_BUTTON,5}},
    {ID_NAVIGATE_SKIPFORWARD, {TBBS_BUTTON,6}},
    {ID_PLAY_FRAMESTEP, {TBBS_BUTTON,7}},
    {ID_DUMMYSEPARATOR, {TBBS_SEPARATOR,-1,true}},
    {ID_VOLUME_MUTE, {TBBS_CHECKBOX,VOLUMEBUTTON_SVG_INDEX,true}},
};

static std::vector<int> supportedSvgButtonsSeq;

IMPLEMENT_DYNAMIC(CPlayerToolBar, CToolBar)
CPlayerToolBar::CPlayerToolBar(CMainFrame* pMainFrame)
    : m_pMainFrame(pMainFrame)
    , m_nButtonHeight(16)
    , m_volumeCtrlSize(60)
    , mouseDownL(false)
    , mouseDownR(false)
    , volumeButtonIndex(12)
    , dummySeparatorIndex(11)
    , flexibleSpaceIndex(10)
    , currentlyDraggingButton(-1)
    , toolbarAdjustActive(false)
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
    auto& s = AfxGetAppSettings();

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

            int volumeIndex = VOLUME_SVG_INDEX;
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
            if (AppIsThemeLoaded() && s.eModernThemeMode == CMPCTheme::ModernThemeMode::DARK) {
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

TBBUTTON CPlayerToolBar::GetStandardButton(int cmdid) {
    auto& svgInfo = supportedSvgButtons[cmdid];
    TBBUTTON button = { 0 };
    button.iBitmap = svgInfo.svgIndex;
    button.idCommand = cmdid;
    button.iString = -1;
    return button;
}

BOOL CPlayerToolBar::Create(CWnd* pParentWnd)
{
    VERIFY(__super::CreateEx(pParentWnd,
                             TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_AUTOSIZE | TBSTYLE_CUSTOMERASE | CCS_ADJUSTABLE,
                             WS_CHILD | WS_VISIBLE | CBRS_BOTTOM /*| CBRS_TOOLTIPS*/,
                             CRect(2, 2, 0, 1)));

    auto& s = AfxGetAppSettings();

    CToolBarCtrl& tb = GetToolBarCtrl();

    dummySeparatorIndex = -1;
    volumeButtonIndex = -1;
    flexibleSpaceIndex = -1;

    for (auto& it : supportedSvgButtons) {
        if (it.second.svgIndex != -1) {
            it.second.text.LoadString(s.CommandIDToWMCMD[it.first]->dwname);
            supportedSvgButtonsSeq.push_back(it.first);
        }
    }

    PlaceButtons(true);

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

void CPlayerToolBar::PlaceButtons(bool loadSavedLayout) {
    CToolBarCtrl& tb = GetToolBarCtrl();

    buttonCount = 0;
    sepCount = 0;

    auto addButton = [&](int cmdid) {
        auto& svgInfo = supportedSvgButtons[cmdid];
        TBBUTTON button = GetStandardButton(cmdid);
        tb.AddButtons(1, &button);
        SetButtonStyle(tb.GetButtonCount() - 1, svgInfo.style | TBBS_DISABLED);
        buttonCount++;
    };

    std::vector<int> buttons(0);
    if (loadSavedLayout) {
        buttons = AfxGetMyApp()->GetProfileVectorInt(L"Toolbars\\PlayerToolBar", L"ButtonSequence");
    }

    addButton(ID_PLAY_PLAY);
    addButton(ID_PLAY_PAUSE);
    addButton(ID_PLAY_STOP);

    if (buttons.size() >= 5) { //it is required that the toolbar have the 5 standard items, otherwise this is invalid
        for (int i = 3; i < buttons.size() - 2; i++) {
            addButton(buttons[i]); //todo: validate these are allowed buttons
        }
    } else { //add standard dynamic items
        addButton(ID_NAVIGATE_SKIPBACK);
        addButton(ID_PLAY_DECRATE);
        addButton(ID_PLAY_INCRATE);
        addButton(ID_NAVIGATE_SKIPFORWARD);
        addButton(ID_PLAY_FRAMESTEP);
    }

    addButton(ID_DUMMYSEPARATOR);
    addButton(ID_VOLUME_MUTE);
}

void CPlayerToolBar::ArrangeControls() {
    if (!::IsWindow(m_volctrl.m_hWnd)) {
        return;
    }

    CRect r;
    GetClientRect(&r);

    CRect br = GetBorders();

    CRect vr;
    float dpiScaling = (float)std::min(m_pMainFrame->m_dpi.ScaleFactorX(), m_pMainFrame->m_dpi.ScaleFactorY());
    int targetsize = int(dpiScaling * AfxGetAppSettings().nDefaultToolbarSize);

    m_volumeCtrlSize = targetsize * 2.5f;
    vr = CRect(r.right + br.right - m_volumeCtrlSize, r.top + targetsize / 4, r.right + br.right, r.bottom - targetsize / 4);

    m_volctrl.MoveWindow(vr);

    volumeButtonIndex = GetToolBarCtrl().GetButtonCount() - 1;
    dummySeparatorIndex = volumeButtonIndex - 1;
    flexibleSpaceIndex = dummySeparatorIndex - 1;

    CRect rFlexible, rVolumeButton;
    GetItemRect(flexibleSpaceIndex, &rFlexible);
    GetItemRect(volumeButtonIndex, &rVolumeButton);
    int spacing = vr.left - rFlexible.right - rVolumeButton.Width();
    SetButtonInfo(dummySeparatorIndex, GetItemID(dummySeparatorIndex), TBBS_SEPARATOR, spacing);
}

void CPlayerToolBar::SetMute(bool fMute) {
    CToolBarCtrl& tb = GetToolBarCtrl();
    TBBUTTONINFO bi = { sizeof(bi) };
    bi.dwMask = TBIF_IMAGE;
    bi.iImage = VOLUMEBUTTON_SVG_INDEX + (fMute ? 1:0);
    tb.SetButtonInfo(ID_VOLUME_MUTE, &bi);
    AfxGetAppSettings().fMute = fMute;
}

bool CPlayerToolBar::IsMuted() const
{
    CToolBarCtrl& tb = GetToolBarCtrl();
    TBBUTTONINFO bi = { sizeof(bi) };
    bi.dwMask = TBIF_IMAGE;
    tb.GetButtonInfo(ID_VOLUME_MUTE, &bi);
    return (bi.iImage == VOLUMEBUTTON_SVG_INDEX + 1);
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
    // button widths are inflated by 7px
    // 9 buttons + 3 separators + spacing + volume
    return buttonCount * (m_nButtonHeight + 1 + 7) + sepCount * (1 + 7) + 4 + m_volumeCtrlSize;
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
    ON_WM_RBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONUP()
    ON_NOTIFY_REFLECT(TBN_QUERYDELETE, &CPlayerToolBar::OnTbnQueryDelete)
    ON_NOTIFY_REFLECT(TBN_QUERYINSERT, &CPlayerToolBar::OnTbnQueryInsert)
    ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, &CPlayerToolBar::OnTbnToolbarChange)
    ON_WM_MOUSEMOVE()
    ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO, &CPlayerToolBar::OnTbnGetButtonInfo)
    ON_NOTIFY_REFLECT(TBN_INITCUSTOMIZE, &CPlayerToolBar::OnTbnInitCustomize)
    ON_NOTIFY_REFLECT(TBN_BEGINADJUST, &CPlayerToolBar::OnTbnBeginAdjust)
    ON_NOTIFY_REFLECT(TBN_ENDADJUST, &CPlayerToolBar::OnTbnEndAdjust)
    ON_WM_LBUTTONDBLCLK()
    ON_NOTIFY_REFLECT(TBN_RESET, &CPlayerToolBar::OnTbnReset)
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
                        drawButtonBG(pTBCD->nmcd, mouseDownL ? CMPCTheme::PlayerButtonClickedColor : CMPCTheme::PlayerButtonHotColor);
                    }
                }
            }
            break;
        case CDDS_ITEMPOSTPAINT:
            // paint over the duplicated separator
            CDC dc;
            dc.Attach(pTBCD->nmcd.hdc);
            RECT r;
            GetItemRect(dummySeparatorIndex, &r);
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
    //ArrangeControls();
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
    mouseDownL = true;

    bool isShift = (MK_SHIFT & nFlags);

    if (isShift) {
        currentlyDraggingButton = i;
    }

    DWORD ignoreButtons = TBBS_SEPARATOR | (isShift ? 0 : TBBS_DISABLED); //if shift is pressed, allow dragging

    if (!m_pMainFrame->m_fFullScreen && (i < 0 || (GetButtonStyle(i) & ignoreButtons))) {
        ClientToScreen(&point);
        m_pMainFrame->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
    } else {
        __super::OnLButtonDown(nFlags, point);
    }
}

void CPlayerToolBar::OnRButtonDown(UINT nFlags, CPoint point) {
    int i = getHitButtonIdx(point);
    mouseDownR = true;

    if (!m_pMainFrame->m_fFullScreen && (i < 0 || (GetButtonStyle(i) & (TBBS_SEPARATOR | TBBS_DISABLED)))) {
        ClientToScreen(&point);
        m_pMainFrame->PostMessage(WM_NCRBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
    } else {
        rightButtonIndex = i;
        __super::OnRButtonDown(nFlags, point);
    }
}

int CPlayerToolBar::getHitButtonIdx(CPoint point)
{
    int hit = -1; // -1 means not on any button hit
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
    if (nID != ID_VOLUME_MUTE && s.CommandIDToWMCMD.count(nID) == 0) {
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
    } else if (bi.iImage == VOLUMEBUTTON_SVG_INDEX) {
        strTipText.LoadString(ID_VOLUME_MUTE);
    } else if (bi.iImage == VOLUMEBUTTON_SVG_INDEX +1 ) {
        strTipText.LoadString(ID_VOLUME_MUTE_OFF);
    } else if (bi.iImage == VOLUMEBUTTON_SVG_INDEX + 2) {
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
    mouseDownL = false;
    CToolBar::OnLButtonUp(nFlags, point);
    m_pMainFrame->RestoreFocus();
}

void CPlayerToolBar::OnRButtonUp(UINT nFlags, CPoint point) {
    CToolBar::OnRButtonUp(nFlags, point);
    mouseDownR = false;

    int buttonId = getHitButtonIdx(point);
    if (buttonId >= 0 && rightButtonIndex == buttonId) {
        int itemId = GetItemID(buttonId);

        UINT messageId = 0;

        switch (itemId) {
            case ID_PLAY_PLAY:
                messageId = ID_FILE_OPENMEDIA;
                break;
            case ID_PLAY_FRAMESTEP:
                messageId = ID_PLAY_FRAMESTEP_BACK;
                break;
            case ID_PLAY_STOP:
                messageId = ID_FILE_CLOSE_AND_RESTORE;
                break;
            case ID_NAVIGATE_SKIPFORWARD:
                messageId = ID_NAVIGATE_SKIPFORWARDFILE;
                break;
            case ID_NAVIGATE_SKIPBACK:
                messageId = ID_NAVIGATE_SKIPBACKFILE;
                break;
            case ID_VOLUME_MUTE:
                messageId = ID_STREAM_AUDIO_NEXT;
                break;
        }

        if (messageId > 0) {
            m_pMainFrame->PostMessage(WM_COMMAND, messageId);
        }
    }
}


void CPlayerToolBar::OnTbnQueryDelete(NMHDR* pNMHDR, LRESULT* pResult) {
    LPNMTOOLBAR pNMTB = reinterpret_cast<LPNMTOOLBAR>(pNMHDR);
    if (pNMTB->iItem < 3 || pNMTB->iItem >= dummySeparatorIndex) {
        *pResult = FALSE;
        return;
    }
    *pResult = TRUE;
}


void CPlayerToolBar::OnTbnQueryInsert(NMHDR* pNMHDR, LRESULT* pResult) {
    LPNMTOOLBAR pNMTB = reinterpret_cast<LPNMTOOLBAR>(pNMHDR);

    bool preventSeparatorInsert = false;

    if (!toolbarAdjustActive) {
        int width = LOWORD(GetToolBarCtrl().GetButtonSize());

        CPoint p = mousePosition;
        p.x += width / 2;

        int testButton = GetToolBarCtrl().HitTest(&p);

        //according to https://learn.microsoft.com/en-us/windows/win32/Controls/tb-hittest
        //"The absolute value of the return value is the index of a separator item"
        //but emperical testing shows it is offset by 1
        if (testButton < 0) {
            testButton = -1 - testButton;
        }

        //undocumented toolbar behavior--if the "testpoint" is on the button to the right, a separator is created to the left
        preventSeparatorInsert = (currentlyDraggingButton + 1 == testButton);
    }

    //this code prevents inserting at the given point
    //first test is to prevent inserting buttons outside of the "dynamic" area
    //second test is to prevent the insertion of a separator directly to the left of the dragged button
    if (pNMTB->iItem < 3 || pNMTB->iItem >= volumeButtonIndex //do not allow moving between beginning and ending standard toolbar items
        || preventSeparatorInsert) 
    {
        *pResult = FALSE;
        return;
    }

    *pResult = TRUE;
}

void CPlayerToolBar::SaveToolbarState() {
    if (!toolbarAdjustActive) {
        auto& ctrl = GetToolBarCtrl();
        std::vector<int> buttons;
        for (int i = 0; i < ctrl.GetButtonCount(); i++) {
            TBBUTTON button;
            ctrl.GetButton(i, &button);
            buttons.push_back(button.idCommand);
        }
        AfxGetMyApp()->WriteProfileVectorInt(L"Toolbars\\PlayerToolBar", L"ButtonSequence", buttons);
    }
}

void CPlayerToolBar::ToolbarChange() {
    SaveToolbarState();
    ArrangeControls();
    Invalidate();
}

void CPlayerToolBar::OnTbnToolbarChange(NMHDR* pNMHDR, LRESULT* pResult) {
    ToolbarChange();
    *pResult = 0;
}

void CPlayerToolBar::OnMouseMove(UINT nFlags, CPoint point) {
    mousePosition = point;
    CToolBar::OnMouseMove(nFlags, point);
}

LPCWSTR CPlayerToolBar::GetStringFromID(int idCommand) {
    return supportedSvgButtons[idCommand].text.GetString();
}

void CPlayerToolBar::OnTbnGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult) {
    LPNMTOOLBAR pNMTB = reinterpret_cast<LPNMTOOLBAR>(pNMHDR);
    if (pNMTB->iItem < supportedSvgButtonsSeq.size()) {
        int cmdid = supportedSvgButtonsSeq[pNMTB->iItem];
        TBBUTTON t = GetStandardButton(cmdid);
        t.iString = (INT_PTR)GetStringFromID(t.idCommand);
        pNMTB->tbButton = t;
        *pResult = TRUE;
        return;
    }
    *pResult = 0;
}


void CPlayerToolBar::OnTbnInitCustomize(NMHDR* pNMHDR, LRESULT* pResult) {
    *pResult = TBNRF_HIDEHELP;
}


void CPlayerToolBar::OnTbnBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult) {
    toolbarAdjustActive = true;
    *pResult = 0;
}

void CPlayerToolBar::OnTbnEndAdjust(NMHDR* pNMHDR, LRESULT* pResult) {
    toolbarAdjustActive = false;
    SaveToolbarState();
    *pResult = 0;
}

void CPlayerToolBar::OnLButtonDblClk(UINT nFlags, CPoint point) {
//disabled to avoid the built-in customization dialog
#if 0    
    m_pMainFrame->enableDialogHook(this, CMainFrame::themableDialogTypes::toolbarCustomizeDialog);
    CToolBar::OnLButtonDblClk(nFlags, point);
#endif
}

void CPlayerToolBar::ToolBarReset() {
    CToolBarCtrl& tb = GetToolBarCtrl();

    for (int i = tb.GetButtonCount() - 1; i >= 0; i--) {
        tb.DeleteButton(i);
    }
    PlaceButtons(false);
    ArrangeControls();
    Invalidate();
}

void CPlayerToolBar::OnTbnReset(NMHDR* pNMHDR, LRESULT* pResult) {
    ToolBarReset();
    *pResult = 0;
}
