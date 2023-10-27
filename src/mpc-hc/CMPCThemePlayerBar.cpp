#include "stdafx.h"
#include "CMPCThemePlayerBar.h"
#include "mplayerc.h"
#include "CMPCTheme.h"
#include "MainFrm.h"

CMPCThemePlayerBar::CMPCThemePlayerBar(CMainFrame* pMainFrame)
    :m_pMainFrame(pMainFrame)
{
    auto& dpi = pMainFrame->m_dpi;
    int buttonDim = CMPCThemeUtil::getConstantByDPI(this, CMPCTheme::ToolbarHideButtonDimensions);
    m_cyGripper = buttonDim + dpi.ScaleX(2);
    m_biHide.SetDpiSize(CSize(buttonDim, buttonDim));
}

CMPCThemePlayerBar::~CMPCThemePlayerBar()
{
}

IMPLEMENT_DYNAMIC(CMPCThemePlayerBar, CPlayerBar)

BEGIN_MESSAGE_MAP(CMPCThemePlayerBar, CPlayerBar)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CMPCThemePlayerBar::OnEraseBkgnd(CDC* pDC)
{
    if (AppIsThemeLoaded()) {
        CRect rect;
        pDC->GetClipBox(&rect);
        pDC->FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), CMPCTheme::WindowBGColor);

        return TRUE;
    } else {
        return __super::OnEraseBkgnd(pDC);
    }
}

void CMPCThemePlayerBar::paintHideButton(CDC* pDC, CSCBButton b) //derived from CSCBButton::Paint
{
    CRect rc = b.GetRect();

    if (b.bPushed) {
        pDC->FillSolidRect(rc, CMPCTheme::ClosePushColor);
    } else if (b.bRaised) {
        pDC->FillSolidRect(rc, CMPCTheme::CloseHoverColor);
    }

    auto& dpi = m_pMainFrame->m_dpi;

    CMPCThemeUtil::drawToolbarHideButton(pDC, this, rc, CMPCThemeUtil::getIconPathByDPI(this, TOOLBAR_HIDE_ICON), dpi.ScaleFactorX(), true);
}

void CMPCThemePlayerBar::NcCalcClient(LPRECT pRc, UINT nDockBarID) { //derived from CSizingControlBarG::NcCalcClient to support DPI changes
    CRect rcBar(pRc); // save the bar rect

    // subtract edges
    baseCSizingControlBarG::NcCalcClient(pRc, nDockBarID);

    if (!HasGripper())
        return;

    CRect rc(pRc); // the client rect as calculated by the base class
    // Work in screen coordinates before converting back to
    // client coordinates to account for possible RTL layout
    GetParent()->ClientToScreen(rcBar);
    GetParent()->ClientToScreen(rc);

    BOOL bHorz = (nDockBarID == AFX_IDW_DOCKBAR_TOP) ||
        (nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);

    if (bHorz)
        rc.DeflateRect(m_cyGripper, 0, 0, 0);
    else
        rc.DeflateRect(0, m_cyGripper, 0, 0);

    auto& dpi = m_pMainFrame->m_dpi;

    // set position for the "x" (hide bar) button
    CPoint ptOrgBtn;
    if (bHorz)
        ptOrgBtn = CPoint(rc.left - dpi.ScaleX(13), rc.top);
    else
        ptOrgBtn = CPoint(rc.right - dpi.ScaleX(12), rc.top - dpi.ScaleY(13));

    m_biHide.Move(ptOrgBtn - rcBar.TopLeft());

    // Work in screen coordinates before converting back to
    // client coordinates to account for possible RTL layout
    GetParent()->ScreenToClient(&rc);

    *pRc = rc;

}

void ResizeBitmap(CBitmap& srcBMP, int dstW, int dstH) {
    CBitmap destBMP;

    BITMAP bm = { 0 };
    srcBMP.GetBitmap(&bm);
    auto size = CSize(bm.bmWidth, bm.bmHeight);
    CWindowDC wndDC(NULL);
    CDC srcDC;
    srcDC.CreateCompatibleDC(&wndDC);
    auto oldSrcBmp = srcDC.SelectObject(&srcBMP);

    CDC destDC;
    destDC.CreateCompatibleDC(&wndDC);
    destBMP.CreateCompatibleBitmap(&wndDC, dstW, dstH);
    auto oldDestBmp = destDC.SelectObject(&destBMP);

    destDC.StretchBlt(0, 0, dstW, dstH, &srcDC, 0, 0, size.cx, size.cy, SRCCOPY);

    srcBMP.DeleteObject();
    srcBMP.Attach(destBMP.Detach());
}

void CMPCThemePlayerBar::NcPaintGripper(CDC* pDC, CRect rcClient)   //derived from CSizingControlBarG base implementation
{
    if (!AppIsThemeLoaded()) {
        __super::NcPaintGripper(pDC, rcClient);
        return;
    }

    if (!HasGripper()) {
        return;
    }

    CRect gripper = rcClient;
    CRect rcbtn = m_biHide.GetRect();
    BOOL bHorz = IsHorzDocked();
    CBitmap patternBMP;

    gripper.DeflateRect(1, 1);
    int gripperHeight = CMPCThemeUtil::getConstantByDPI(this, CMPCTheme::ToolbarGripperHeight);

    if (bHorz) {   // gripper at left
        gripper.left = rcbtn.left + (rcbtn.Width() - gripperHeight) / 2;
        gripper.top = rcbtn.bottom + 3;
    } else {   // gripper at top
        gripper.top = rcbtn.top + (rcbtn.Height() - gripperHeight) / 2;
        gripper.right = rcbtn.left - 3;
    }
    CMPCThemeUtil::drawGripper(this, gripper, pDC, bHorz);

    paintHideButton(pDC, m_biHide);
}

void CMPCThemePlayerBar::mpc_fillNcBG(CDC* mdc, CRect rcDraw)
{
    if (AppIsThemeLoaded()) {
        if (IsFloating()) {
            rcDraw.DeflateRect(1, 1);
        }
        mdc->FillSolidRect(rcDraw, CMPCTheme::WindowBGColor);
    } else {
        __super::mpc_fillNcBG(mdc, rcDraw);
    }
}
