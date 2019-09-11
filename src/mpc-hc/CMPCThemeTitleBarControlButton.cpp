#include "stdafx.h"
#include "CMPCThemeTitleBarControlButton.h"

BEGIN_MESSAGE_MAP(CMPCThemeTitleBarControlButton, CMFCButton)
    ON_WM_PAINT()
END_MESSAGE_MAP()

CMPCThemeTitleBarControlButton::CMPCThemeTitleBarControlButton(LRESULT _buttonType) : CMFCButton()
, parent(nullptr) {
    this->buttonType = _buttonType;
    switch (buttonType) {
    case HTCLOSE:
        hoverColor = CMPCTheme::CloseHoverColor;
        pushedColor = CMPCTheme::ClosePushColor;
        break;
    case HTMINBUTTON:
    case HTMAXBUTTON:
    default:
        hoverColor = CMPCTheme::W10DarkThemeTitlebarControlHoverBGColor;
        pushedColor = CMPCTheme::W10DarkThemeTitlebarControlPushedBGColor;
        break;
    }
}

const std::vector<CMPCTheme::pathPoint>& CMPCThemeTitleBarControlButton::getIconPath() {
    switch (buttonType) {
    case HTMINBUTTON:
        return CMPCTheme::minimizeIcon;
    case HTMAXBUTTON:
        return CMPCTheme::maximizeIcon;
    case HTCLOSE:
    default:
        return CMPCTheme::closeIcon;
    }
}

void CMPCThemeTitleBarControlButton::setParentFrame(CMPCThemeFrameUtil* _parent) {
    this->parent = _parent;
}

void CMPCThemeTitleBarControlButton::drawTitleBarButton(CDC* pDC, CRect iconRect, std::vector<CMPCTheme::pathPoint> icon, double dpiScaling, bool antiAlias) {
    CRect pathRect = {
        iconRect.left + (iconRect.Width() - CMPCTheme::W10TitlebarIconPathWidth) / 2,
        iconRect.top + (iconRect.Height() - CMPCTheme::W10TitlebarIconPathHeight) / 2,
        CMPCTheme::W10TitlebarIconPathWidth,
        CMPCTheme::W10TitlebarIconPathHeight
    };

    Gdiplus::Graphics gfx(pDC->m_hDC);
    if (antiAlias) {
        gfx.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
    }
    Gdiplus::Color lineClr;
    lineClr.SetFromCOLORREF(CMPCTheme::W10DarkThemeTitlebarIconPenColor);
    Gdiplus::Pen iPen(lineClr, 1.5);
    Gdiplus::REAL lastX = 0, lastY = 0;
    for (u_int i = 0; i < icon.size(); i++) {
        CMPCTheme::pathPoint p = icon[i];
        Gdiplus::REAL x = (Gdiplus::REAL)(pathRect.left + p.x);
        Gdiplus::REAL y = (Gdiplus::REAL)(pathRect.top + p.y);
        if (p.state == CMPCTheme::newPath) {
            lastX = x;
            lastY = y;
        } else if ((p.state == CMPCTheme::linePath || p.state == CMPCTheme::closePath) && i > 0) {
            gfx.DrawLine(&iPen, lastX, lastY, x, y);
            if (antiAlias) {
                gfx.DrawLine(&iPen, lastX, lastY, x, y); //draw again to brighten the AA diagonals
            }
            lastX = x;
            lastY = y;
        }
    }
}


void CMPCThemeTitleBarControlButton::OnPaint() {
    CPaintDC dc(this); // device context for painting
    CRect cr;
    GetClientRect(cr);
    if (IsPushed()) {
        dc.FillSolidRect(cr, pushedColor);
    } else if (IsHighlighted()) {
        dc.FillSolidRect(cr, hoverColor);
    } else {
        if (nullptr != parent) {
            dc.FillSolidRect(cr, parent->getTitleBarColor());
        }
    }
    DpiHelper dpiWindow;
    dpiWindow.Override(AfxGetMainWnd()->GetSafeHwnd());
    drawTitleBarButton(&dc, cr, getIconPath(), dpiWindow.ScaleFactorX(), true);
}
