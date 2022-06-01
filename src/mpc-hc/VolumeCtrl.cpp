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
#include "VolumeCtrl.h"
#include "AppSettings.h"
#include "CMPCTheme.h"
#include "PlayerToolBar.h"
#include "CMPCThemeUtil.h"
#undef SubclassWindow


// CVolumeCtrl

IMPLEMENT_DYNAMIC(CVolumeCtrl, CSliderCtrl)
CVolumeCtrl::CVolumeCtrl(bool fSelfDrawn)
    : m_fSelfDrawn(fSelfDrawn)
    , m_bDrag(false)
    , m_bHover(false)
    , modernStyle(AfxGetAppSettings().bModernSeekbar)
{
}

CVolumeCtrl::~CVolumeCtrl()
{
}

bool CVolumeCtrl::Create(CWnd* pParentWnd)
{
    if (!CSliderCtrl::Create(WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_HORZ | TBS_TOOLTIPS, CRect(0, 0, 0, 0), pParentWnd, IDC_SLIDER1)) {
        return false;
    }

    const CAppSettings& s = AfxGetAppSettings();
    EnableToolTips(TRUE);
    SetRange(0, 100);
    SetPos(s.nVolume);
    SetPageSize(s.nVolumeStep);
    SetLineSize(0);

    if (AppIsThemeLoaded()) {
        CToolTipCtrl* pTip = GetToolTips();
        if (NULL != pTip) {
            themedToolTip.SubclassWindow(pTip->m_hWnd);
        }
    }

    return true;
}

void CVolumeCtrl::SetPosInternal(int pos)
{
    SetPos(pos);
    GetParent()->PostMessage(WM_HSCROLL, MAKEWPARAM(static_cast<WORD>(pos), SB_THUMBPOSITION), reinterpret_cast<LPARAM>(m_hWnd)); // this will be reflected back on us
    POINT p;
    ::GetCursorPos(&p);
    ScreenToClient(&p);
    checkHover(p);
}

void CVolumeCtrl::IncreaseVolume()
{
    // align volume up to step. recommend using steps 1, 2, 5 and 10
    SetPosInternal(GetPos() + GetPageSize() - GetPos() % GetPageSize());

}

void CVolumeCtrl::DecreaseVolume()
{
    // align volume down to step. recommend using steps 1, 2, 5 and 10
    int m = GetPos() % GetPageSize();
    SetPosInternal(GetPos() - (m ? m : GetPageSize()));
}

BEGIN_MESSAGE_MAP(CVolumeCtrl, CSliderCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
    ON_WM_LBUTTONDOWN()
    ON_WM_SETFOCUS()
    ON_WM_HSCROLL_REFLECT()
    ON_WM_SETCURSOR()
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CVolumeCtrl message handlers

void CVolumeCtrl::getCustomChannelRect(LPRECT rc)
{
    CRect channelRect;
    GetChannelRect(channelRect);
    CRect thumbRect;
    GetThumbRect(thumbRect);

    CopyRect(rc, CRect(channelRect.left, thumbRect.top + 2, channelRect.right - 2, thumbRect.bottom - 2));
}

void CVolumeCtrl::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

    LRESULT lr = CDRF_DODEFAULT;

    bool usetheme = AppIsThemeLoaded();

    if (m_fSelfDrawn)
        switch (pNMCD->dwDrawStage) {
            case CDDS_PREPAINT:
                lr = CDRF_NOTIFYITEMDRAW;
                break;

            case CDDS_ITEMPREPAINT:

                if (pNMCD->dwItemSpec == TBCD_CHANNEL) {
                    CRect rect;
                    GetClientRect(rect);

                    CDC dc;
                    dc.Attach(pNMCD->hdc);
                    CDC dcMem;
                    CBitmap bmMem;
                    CRect memRect = { 0, 0, rect.right, rect.bottom };
                    CMPCThemeUtil::initMemDC(&dc, dcMem, bmMem, memRect);

                    dcMem.FillSolidRect(&rect, CMPCTheme::PlayerBGColor);

                    DpiHelper dpiWindow;
                    dpiWindow.Override(GetSafeHwnd());

                    CPlayerToolBar* tb = DYNAMIC_DOWNCAST(CPlayerToolBar, GetParent());
                    if (tb) {
                        CImage& vOn = tb->GetVolumeImageOn();
                        CImage& vOff = tb->GetVolumeImageOff();
                        CRect dr, drOn, drOff, src;
                        dr.SetRect(0, 0, vOn.GetWidth(), vOn.GetHeight());
                        dr.OffsetRect((rect.Width() - vOn.GetWidth()) / 2, (rect.Height() - vOn.GetHeight()) / 2);
                        drOn = dr;
                        drOn.right = dr.left + lround(dr.Width() * float(GetPos()) / 100);
                        if (drOn.Width() > 0) {
                            src = drOn;
                            src.OffsetRect(-src.TopLeft());
                            vOn.Draw(dcMem, drOn, src);
                        }
                        drOff = dr;
                        drOff.left = drOn.right;
                        if (drOff.Width() > 0) {
                            src = drOff;
                            src.OffsetRect(-src.TopLeft());
                            src.OffsetRect(drOn.right-drOn.left, 0);
                            vOff.Draw(dcMem, drOff, src);
                        }
                    }
                    CMPCThemeUtil::flushMemDC(&dc, dcMem, memRect);
                    dc.Detach();
                    lr = CDRF_SKIPDEFAULT;
                } else if (pNMCD->dwItemSpec == TBCD_THUMB) {
                    lr = CDRF_SKIPDEFAULT;
                }

                break;
        };

    pNMCD->uItemState &= ~CDIS_FOCUS;

    *pResult = lr;
}

void CVolumeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect r;
    GetChannelRect(&r);

    if (r.left >= r.right) {
        return;
    }

    int start, stop;
    GetRange(start, stop);

    if (!(AppIsThemeLoaded() && modernStyle)) {
        r.left += 3;
        r.right -= 4;
    }

    if (point.x < r.left) {
        SetPosInternal(start);
    } else if (point.x >= r.right) {
        SetPosInternal(stop);
    } else {
        int w = r.right - r.left;
        if (start < stop) {
            if (!(AppIsThemeLoaded() && modernStyle)) {
                SetPosInternal(start + ((stop - start) * (point.x - r.left) + (w / 2)) / w);
            } else {
                SetPosInternal(start + lround((stop - start) * float(point.x - r.left) / w));
            }
        }
    }
    m_bDrag = true;
    if (AppIsThemeLoaded() && modernStyle) {
        if (themedToolTip.m_hWnd) {
            TOOLINFO ti = { sizeof(TOOLINFO) };
            ti.uFlags = TTF_TRACK | TTF_IDISHWND | TTF_ABSOLUTE;
            ti.hwnd = m_hWnd;
            ti.uId = (UINT_PTR)m_hWnd;
            ti.hinst = AfxGetInstanceHandle();
            ti.lpszText = LPSTR_TEXTCALLBACK;

            themedToolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
        }

        updateModernVolCtrl(point);
        SetCapture();
    } else {
        invalidateThumb();
        CSliderCtrl::OnLButtonDown(nFlags, point);
    }
}

void CVolumeCtrl::OnSetFocus(CWnd* pOldWnd)
{
    CSliderCtrl::OnSetFocus(pOldWnd);

    AfxGetMainWnd()->SetFocus(); // don't focus on us, parents will take care of our positioning
}

void CVolumeCtrl::HScroll(UINT nSBCode, UINT nPos)
{
    AfxGetAppSettings().nVolume = GetPos();

    CFrameWnd* pFrame = GetParentFrame();
    if (pFrame && pFrame != GetParent()) {
        pFrame->PostMessage(WM_HSCROLL, MAKEWPARAM(static_cast<WORD>(nPos), static_cast<WORD>(nSBCode)), reinterpret_cast<LPARAM>(m_hWnd));
    }
}

BOOL CVolumeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
    return TRUE;
}

BOOL CVolumeCtrl::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
    TOOLTIPTEXT* pTTT = reinterpret_cast<LPTOOLTIPTEXT>(pNMHDR);
    CString str;
    str.Format(IDS_VOLUME, GetPos());
    _tcscpy_s(pTTT->szText, str);
    pTTT->hinst = nullptr;

    *pResult = 0;

    return TRUE;
}

BOOL CVolumeCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
    if (zDelta > 0) {
        IncreaseVolume();
    } else if (zDelta < 0) {
        DecreaseVolume();
    } else {
        return FALSE;
    }
    return TRUE;
}

void CVolumeCtrl::invalidateThumb()
{
    if (!(AppIsThemeLoaded() && modernStyle)) {
        SetRangeMax(100, TRUE);
    }
}


void CVolumeCtrl::checkHover(CPoint point)
{
    CRect thumbRect;
    GetThumbRect(thumbRect);
    bool oldHover = m_bHover;
    m_bHover = false;
    if (thumbRect.PtInRect(point)) {
        m_bHover = true;
    }

    if (m_bHover != oldHover) {
        invalidateThumb();
    }
}

void CVolumeCtrl::updateModernVolCtrl(CPoint point)
{
    //CSliderCtrl::OnMouseMove yields bad results due to assumption of thumb width
    //we must do all position calculation ourselves, and send correct position to tooltip

    CRect r;
    GetChannelRect(&r);

    int start, stop;
    GetRange(start, stop);
    int useX;
    if (point.x < r.left) {
        SetPosInternal(start);
        useX = r.left;
    } else if (point.x >= r.right) {
        SetPosInternal(stop);
        useX = r.right;
    } else {
        int w = r.right - r.left;
        if (start < stop) {
            SetPosInternal(start + lround((stop - start) * float(point.x - r.left) / w));
        }
        useX = point.x;
    }
    POINT p = { useX, point.y };
    ClientToScreen(&p);
    CRect ttRect;
    if (themedToolTip.m_hWnd) {
        CRect cr = r;
        ClientToScreen(cr);
        themedToolTip.GetWindowRect(ttRect);
        p.y = cr.top - ttRect.Height();
        themedToolTip.SendMessage(TTM_TRACKPOSITION, 0, MAKELPARAM(p.x, p.y));
    }

    RECT ur;
    //getCustomChannelRect(&ur);
    GetClientRect(&ur);
    RedrawWindow(&ur, nullptr, RDW_INVALIDATE); //we must redraw the whole channel with the modern volume ctrl. by default only areas where thumb has been are invalidated
}


void CVolumeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    checkHover(point);

    if (AppIsThemeLoaded() && modernStyle && m_bDrag) {
        updateModernVolCtrl(point);
    } else {
        CSliderCtrl::OnMouseMove(nFlags, point);
    }
}


void CVolumeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (AppIsThemeLoaded() && modernStyle) {
        if (m_bDrag) {
            ReleaseCapture();
        }
        m_bDrag = false;
        if (themedToolTip.m_hWnd) {
            themedToolTip.SendMessage(TTM_TRACKACTIVATE, FALSE, 0);
        }
    } else {
        m_bDrag = false;
        invalidateThumb();
        checkHover(point);
        CSliderCtrl::OnLButtonUp(nFlags, point);
    }
}


void CVolumeCtrl::OnMouseLeave()
{
    checkHover(CPoint(-1 - 1));
    CSliderCtrl::OnMouseLeave();
}
