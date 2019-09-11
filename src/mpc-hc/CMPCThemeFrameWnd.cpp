#include "stdafx.h"
#include "CMPCThemeFrameWnd.h"
#include "CMPCTheme.h"
#include "CMPCThemeUtil.h"
#include "mplayerc.h"
#include "SVGImage.h"
#include <gdiplusgraphics.h>

IMPLEMENT_DYNAMIC(CMPCThemeFrameWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CMPCThemeFrameWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_NCHITTEST()
	ON_WM_NCMOUSELEAVE()
END_MESSAGE_MAP()


CMPCThemeFrameWnd::CMPCThemeFrameWnd():
    minimizeButton(HTMINBUTTON),
    maximizeButton(HTMAXBUTTON),
    closeButton(HTCLOSE),
    currentFrameState(frameNormal),
    wantFrameState(frameNormal),
    frameChanged(false),
    titleBarInfo({ 0 }),
    disableFrameAdjust(false),
    titlebarHeight(30) //sane default, should be updated as soon as created
{
}

CMPCThemeFrameWnd::~CMPCThemeFrameWnd() {
}

void CMPCThemeFrameWnd::RecalcLayout(BOOL bNotify) {
    recalcTitleBar();
    CRect cr = getTitleBarRect();
    if (wantFrameState == frameThemed)  {
        m_rectBorder.top = cr.bottom + GetSystemMetrics(SM_CYMENU);

        CRect sysMenuIconRect = getSysMenuIconRect();
        CRect closeRect, maximizeRect, minimizeRect;
        CRect titleBarRect = getTitleBarRect();
        GetIconRects(titleBarRect, closeRect, maximizeRect, minimizeRect);

        if (IsWindow(closeButton.m_hWnd)) {
            closeButton.MoveWindow(closeRect, TRUE);
            closeButton.ShowWindow(SW_SHOW);
        }
        if (IsWindow(minimizeButton.m_hWnd)) {
            minimizeButton.MoveWindow(minimizeRect, TRUE);
            minimizeButton.ShowWindow(SW_SHOW);
        }
        if (IsWindow(maximizeButton.m_hWnd)) {
            maximizeButton.MoveWindow(maximizeRect, TRUE);
            maximizeButton.ShowWindow(SW_SHOW);
        }
    } else {
        m_rectBorder.top = borders.top;
        if (IsWindow(closeButton.m_hWnd)) {
            closeButton.ShowWindow(SW_HIDE);
        }
        if (IsWindow(minimizeButton.m_hWnd)) {
            minimizeButton.ShowWindow(SW_HIDE);
        }
        if (IsWindow(maximizeButton.m_hWnd)) {
            maximizeButton.ShowWindow(SW_HIDE);
        }
    }
	__super::RecalcLayout(bNotify);
    Invalidate(FALSE);
}

CRect CMPCThemeFrameWnd::getTitleBarRect() {
    CRect cr;
    GetClientRect(&cr);
    if (IsZoomed()) {
        cr.top += borders.top / 2; //invisible area when maximized
    } else {
        cr.top += 1; //border
    }
    cr.bottom = cr.top + titlebarHeight;
    return cr;
}

CRect CMPCThemeFrameWnd::getSysMenuIconRect() {
    CRect sysMenuIconRect, cr;
    cr = getTitleBarRect();

    DpiHelper dpiWindow;
    dpiWindow.Override(AfxGetMainWnd()->GetSafeHwnd());
    int iconsize = MulDiv(dpiWindow.DPIX(), 1, 6);
    sysMenuIconRect.top = cr.top + (cr.Height() - iconsize) / 2 + 1;
    sysMenuIconRect.bottom = sysMenuIconRect.top + iconsize;
    sysMenuIconRect.left = (dpiWindow.ScaleX(30) - iconsize) / 2 + 1;
    sysMenuIconRect.right = sysMenuIconRect.left + iconsize;
    return sysMenuIconRect;
}

void CMPCThemeFrameWnd::checkFrame(LONG style) {
    wantFrameState = frameNormal;
    if ((WS_CAPTION | WS_THICKFRAME) == (style & (WS_CAPTION | WS_THICKFRAME)) && !disableFrameAdjust) {
        if (AfxGetAppSettings().bMPCThemeLoaded && IsWindows10OrGreater()) {
            wantFrameState = frameThemed;
        }
    }
}

int CMPCThemeFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    checkFrame(lpCreateStruct->style);

    if (wantFrameState == frameThemed) {
        int res = CWnd::OnCreate(lpCreateStruct);

        if (res == -1)
            return -1;

        RECT r = { 0,0,0,0 };

        closeButton.Create(_T("Close Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, r, this, 1001);
        closeButton.setParentFrame(this);

        minimizeButton.Create(_T("Minimize Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, r, this, 1002);
        minimizeButton.setParentFrame(this);

        maximizeButton.Create(_T("Maximize Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, r, this, 1003);
        maximizeButton.setParentFrame(this);

        return res;
    } else {
        return __super::OnCreate(lpCreateStruct);
    }
}

void CMPCThemeFrameWnd::GetIconRects(CRect titlebarRect, CRect& closeRect, CRect& maximizeRect, CRect& minimizeRect) {
    int closeRightX;
    if (IsZoomed()) {
        closeRightX = titlebarRect.right - 2;
    } else {
        closeRightX = titlebarRect.right;
    }

    CRect iconDimRect(0, 0, CMPCTheme::W10TitlebarIconWidth, titlebarRect.Height() - 1);

    closeRect = CRect(closeRightX - iconDimRect.Width(), titlebarRect.top, closeRightX, titlebarRect.top + iconDimRect.Height());
    maximizeRect = CRect(closeRect.left - 1 - iconDimRect.Width(), titlebarRect.top, closeRect.left - 1, titlebarRect.top + iconDimRect.Height());
    minimizeRect = CRect(maximizeRect.left - 1 - iconDimRect.Width(), titlebarRect.top, maximizeRect.left - 1, titlebarRect.top + iconDimRect.Height());
}

void CMPCThemeFrameWnd::OnPaint() {
	if (currentFrameState == frameThemed) {
		CPaintDC dc(this);
        CRect titleBarRect = getTitleBarRect();

        CDC dcMem;
        CBitmap bmMem;
        CRect memRect = titleBarRect;
        memRect.top -= 1;
        CMPCThemeUtil::initMemDC(&dc, dcMem, bmMem, memRect);

        CRect topBorderRect = { titleBarRect.left, titleBarRect.top - 1, titleBarRect.right, titleBarRect.top };
        dcMem.FillSolidRect(topBorderRect, CMPCTheme::W10DarkThemeWindowBorderColor);

        dcMem.FillSolidRect(titleBarRect, titleBarColor);
        //dcMem.FillSolidRect(cr, CMPCTheme::DebugColorRed);

        CFont f;
        CMPCThemeUtil::getFontByType(f, &dcMem, CMPCThemeUtil::CaptionFont);
        dcMem.SelectObject(f);

        CRect captionRect = titleBarRect;
        DpiHelper dpi = DpiHelper();
        dpi.Override(AfxGetMainWnd()->GetSafeHwnd());
        captionRect.left += dpi.ScaleX(30);
        captionRect.right -= dpi.ScaleX(125);

        CFont font;
        CMPCThemeUtil::getFontByType(font, &dcMem, CMPCThemeUtil::CaptionFont);
        dcMem.SetBkColor(titleBarColor);
        dcMem.SetTextColor(CMPCTheme::W10DarkThemeTitlebarFGColor);
        dcMem.DrawText(m_strTitle, captionRect, DT_LEFT | DT_WORD_ELLIPSIS | DT_VCENTER | DT_SINGLELINE);

        CRect sysMenuIconRect = getSysMenuIconRect();
        int sysIconDim = sysMenuIconRect.Width();
        HICON icon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_nIDHelp), IMAGE_ICON, sysIconDim, sysIconDim, LR_SHARED);
        ::DrawIconEx(dcMem.m_hDC, sysMenuIconRect.left, sysMenuIconRect.top, icon, 0, 0, 0, nullptr, DI_NORMAL);

        CMPCThemeUtil::flushMemDC(&dc, dcMem, memRect);

		if (m_pCtrlCont != NULL) {
			m_pCtrlCont->OnPaint(&dc);
		}
		Default();
	} else {
		__super::OnPaint();
	}

}

void CMPCThemeFrameWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    checkFrame(GetStyle());
    if (bCalcValidRects) {
        if (wantFrameState == frameThemed) {
            lpncsp->rgrc[0].left += borders.left + 1;
            lpncsp->rgrc[0].right -= borders.right + 1;
            lpncsp->rgrc[0].bottom -= borders.bottom + 1;
        } else {
            __super::OnNcCalcSize(bCalcValidRects, lpncsp);
            /*
            lpncsp->rgrc[0].left += 0;
            lpncsp->rgrc[0].right += 0;
            lpncsp->rgrc[0].bottom += 0;
            */
        }

        if (frameChanged) {
            currentFrameState = wantFrameState;
            frameChanged = false;
        }
    } else {
        __super::OnNcCalcSize(bCalcValidRects, lpncsp);
    }
}

void CMPCThemeFrameWnd::recalcFrame() {
    checkFrame(GetStyle());
    if (wantFrameState != currentFrameState) {
        borders = { 0,0,0,0 };
        UINT style = GetStyle();
        if (0 != (style & WS_THICKFRAME)) {
            AdjustWindowRectEx(&borders, style & ~WS_CAPTION, FALSE, NULL);
            borders.left = abs(borders.left);
            borders.top = abs(borders.top);
        } else if (0 != (style & WS_BORDER)) {
            borders = { 1,1,1,1 };
        }

        if (wantFrameState == frameThemed) {
            frameChanged = true;
            SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
            if (GetForegroundWindow() == this) {
                titleBarColor = CMPCTheme::W10DarkThemeTitlebarBGColor;
            } else {
                titleBarColor = CMPCTheme::W10DarkThemeTitlebarInactiveBGColor;
            }
        } else {
            frameChanged = true;
            SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
        }
    }
}

void CMPCThemeFrameWnd::allowMPCThemeFrameAdjust(bool allow) {
    if (allow) {
        disableFrameAdjust = false;
        recalcFrame();
    } else {
        disableFrameAdjust = true;
    }
}


void CMPCThemeFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) {
    if (titleBarInfo.cbSize == 0) { //only check this once, as it can be wrong later
        titleBarInfo = { sizeof(TITLEBARINFO) };
        GetTitleBarInfo(&titleBarInfo);
    }
    recalcFrame();
    CWnd::OnActivate(nState, pWndOther, bMinimized);
    Invalidate(TRUE);
}

LRESULT CMPCThemeFrameWnd::OnNcHitTest(CPoint point) {
	if (currentFrameState == frameThemed) {
		LRESULT result = 0;

        result = CWnd::OnNcHitTest(point);
		if (result == HTCLIENT) {
			ScreenToClient(&point);
            if (point.y < borders.top) {
                return HTTOP;
            } else if (point.y < titlebarHeight) {
                CRect sysMenuIconRect = getSysMenuIconRect();
                CRect closeRect, maximizeRect, minimizeRect;
                CRect titleBarRect = getTitleBarRect();
                GetIconRects(titleBarRect, closeRect, maximizeRect, minimizeRect);

                if (sysMenuIconRect.PtInRect(point)) {
                    return HTSYSMENU;
                } else if (closeRect.PtInRect(point) || minimizeRect.PtInRect(point) || maximizeRect.PtInRect(point)) {
                    return HTNOWHERE;
                } else {
                    return HTCAPTION;
                }
            } else if (point.y < titlebarHeight + GetSystemMetrics(SM_CYMENU)) {
                return HTMENU;
            }
		}
		return result;
	} else {
		return __super::OnNcHitTest(point);
	}
}

void CMPCThemeFrameWnd::OnNcMouseLeave() {
    if (currentFrameState == frameThemed) {
        CWnd::OnNcMouseLeave();
	} else {
		__super::OnNcMouseLeave();
	}
}

void CMPCThemeFrameWnd::recalcTitleBar() {
    titlebarHeight = CRect(titleBarInfo.rcTitleBar).Height() + borders.top;
    if (IsZoomed()) {
        titlebarHeight -= borders.top / 2;
    }
}
