#include "stdafx.h"
#include "CMPCThemePlayerListCtrl.h"
#include "CMPCTheme.h"
#include "CMPCThemeUtil.h"
#include "mplayerc.h"
#undef SubclassWindow

CMPCThemePlayerListCtrl::CMPCThemePlayerListCtrl() : CListCtrl()
{
    themeGridLines = false;
    fullRowSelect = false;
    themedSBHelper = nullptr;
    clipChildWindows = false;
    hasCheckedColors = false;
    hasCBImages = false;
    customThemeInterface = nullptr;
    if (!CMPCThemeUtil::canUseWin10DarkTheme()) {
        themedSBHelper = DEBUG_NEW CMPCThemeScrollBarHelper(this);
    }
}


CMPCThemePlayerListCtrl::~CMPCThemePlayerListCtrl()
{
    if (nullptr != themedSBHelper) {
        delete themedSBHelper;
    }
}


void CMPCThemePlayerListCtrl::PreSubclassWindow()
{
    if (!AppNeedsThemedControls()) {
        EnableToolTips(TRUE);
    } else {
        if (CMPCThemeUtil::canUseWin10DarkTheme()) {
            SetWindowTheme(GetSafeHwnd(), L"DarkMode_Explorer", NULL);
        } else {
            SetWindowTheme(GetSafeHwnd(), L"", NULL);
        }
        CToolTipCtrl* t = GetToolTips();
        if (nullptr != t) {
            lvsToolTip.SubclassWindow(t->m_hWnd);
        }
        subclassHeader();
    }
    CListCtrl::PreSubclassWindow();
}

IMPLEMENT_DYNAMIC(CMPCThemePlayerListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CMPCThemePlayerListCtrl, CListCtrl)
    ON_WM_PAINT()
    ON_WM_NCPAINT()
    ON_WM_CREATE()
    ON_NOTIFY_REFLECT_EX(LVN_ENDSCROLL, OnLvnEndScroll)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_NCCALCSIZE()
    ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, OnCustomDraw)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_NOTIFY_EX(HDN_ENDTRACKA, 0, &OnHdnEndtrack)
    ON_NOTIFY_EX(HDN_ENDTRACKW, 0, &OnHdnEndtrack)
    ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, &OnLvnItemchanged)
    ON_MESSAGE(PLAYER_PLAYLIST_UPDATE_SCROLLBAR, OnDelayed_UpdateScrollbar)
    ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

void CMPCThemePlayerListCtrl::OnWindowPosChanged(WINDOWPOS* lpwndpos) {
    if (AppNeedsThemedControls()) {
        if (themedSBHelper) {
            if (0 != (GetStyle() & (WS_VSCROLL | WS_HSCROLL))) {
                themedSBHelper->OnWindowPosChanged();
            } else {
                themedSBHelper->InvalidateScrollbarArea();
            }
        }
    }
    return __super::OnWindowPosChanged(lpwndpos);
}


void CMPCThemePlayerListCtrl::subclassHeader()
{
    CHeaderCtrl* t = GetHeaderCtrl();
    if (nullptr != t && IsWindow(t->m_hWnd) && themedHdrCtrl.m_hWnd == NULL) {
        themedHdrCtrl.SetParent(this);
        themedHdrCtrl.SubclassWindow(t->GetSafeHwnd());
    }
}

void CMPCThemePlayerListCtrl::ExcludeChildWindows(CDC* pDC, CRgn* pClipRgn) {
    if (clipChildWindows) {
        // Enumerate all child windows and exclude them from clipping (except the header)
        CWnd* pChild = GetWindow(GW_CHILD);
        while (pChild != NULL) {
            // Check if child window is visible
            if (pChild->IsWindowVisible()) {
                CRect childRect;
                pChild->GetWindowRect(&childRect);
                ScreenToClient(&childRect);

                // Create region for this child window
                CRgn childRgn;
                childRgn.CreateRectRgnIndirect(&childRect);

                // Exclude this child from our clipping region
                pClipRgn->CombineRgn(pClipRgn, &childRgn, RGN_DIFF);
            }

            pChild = pChild->GetWindow(GW_HWNDNEXT);
        }
    }
}

CPoint DetectHeaderOffset(CListCtrl* pList, CHeaderCtrl* pHeader) {
    CRect columnRect, headerItemRect, headerWindowRect;

    if (!pList->GetSubItemRect(0, 0, LVIR_BOUNDS, columnRect) ||
        !pHeader->GetItemRect(0, headerItemRect)) {
        return CPoint(0, 0);
    }

    pHeader->GetWindowRect(&headerWindowRect);
    pList->ScreenToClient(&headerWindowRect);

    int offset = columnRect.left - (headerWindowRect.left + headerItemRect.left);
    return CPoint(offset, 0);
}

bool CMPCThemePlayerListCtrl::IsCustomDrawActive() {
    if (!::IsWindow(m_hWnd)) {
        return false;
    }

    CWnd* pParent = GetParent();
    if (!pParent) {
        return false;
    }

    NMLVCUSTOMDRAW nmcd = { 0 };
    nmcd.nmcd.hdr.hwndFrom = m_hWnd;
    nmcd.nmcd.hdr.idFrom = GetDlgCtrlID();
    nmcd.nmcd.hdr.code = NM_CUSTOMDRAW;
    nmcd.nmcd.dwDrawStage = CDDS_PREPAINT;

    LRESULT lResult = pParent->SendMessage(WM_NOTIFY,  nmcd.nmcd.hdr.idFrom,  (LPARAM)&nmcd);

    return (lResult != CDRF_DODEFAULT);
}

bool CMPCThemePlayerListCtrl::PaintHooksActive() {
    return (GetStyle() & (LVS_OWNERDRAWFIXED)) != 0 || IsCustomDrawActive();
}

void CMPCThemePlayerListCtrl::RedrawHeader() {
    if (themedHdrCtrl) {
        CRect headerRect;
        themedHdrCtrl.GetWindowRect(&headerRect);
        ScreenToClient(&headerRect);
        RedrawWindow(headerRect, 0, RDW_INVALIDATE);
    }
}

void CMPCThemePlayerListCtrl::OnPaint() {
  if (AppNeedsThemedControls() && !PaintHooksActive()) {
    CPaintDC dc(this);
    CRect updateRect;
    dc.GetClipBox(&updateRect);
    if (updateRect.IsRectEmpty()) {
        return;
    }

    // Setup
    CRect clientRect;
    GetClientRect(&clientRect);
    CPoint clientTopLeft(0, 0);
    ClientToScreen(&clientTopLeft);

    if (nullptr != customThemeInterface) {
        customThemeInterface->DoCustomPrePaint();
    }

    CRect listArea = clientRect;
    bool hasHeader = themedHdrCtrl && themedHdrCtrl.IsWindowVisible();
    CRect headerRect(0, 0, 0, 0);
    if (hasHeader) {
        themedHdrCtrl.GetWindowRect(&headerRect);
        ScreenToClient(&headerRect);
        listArea.top = headerRect.bottom;
    }

    m_listBuffer.EnsureInitialized(&dc, listArea.Size(), GetFont());
    CRect drawRect = m_listBuffer.bValid ? updateRect : clientRect;
    m_listBuffer.bValid = true;

    // Create header DC and draw if needed
    if (hasHeader) {
        m_headerBuffer.EnsureInitialized(&dc, headerRect.Size(), GetFont());
        themedHdrCtrl.DrawAllItems(&m_headerBuffer.memDC, DetectHeaderOffset(this, &themedHdrCtrl));
    }

    // Draw list items
    CRect listDrawRect;
    listDrawRect.IntersectRect(&drawRect, &listArea);
    if (!listDrawRect.IsRectEmpty()) {
        EraseBkgnd(&m_listBuffer.memDC);
        DrawAllItems(&m_listBuffer.memDC, listDrawRect);
    }

    // Blit list
    if (!listDrawRect.IsRectEmpty()) {
        CRect listClipRect = listDrawRect;
        CRgn listClipRgn;
        listClipRgn.CreateRectRgnIndirect(&listClipRect);
        ExcludeChildWindows(&dc, &listClipRgn);
        dc.SelectClipRgn(&listClipRgn);
        dc.BitBlt(listDrawRect.left, listDrawRect.top, listDrawRect.Width(), listDrawRect.Height(), &m_listBuffer.memDC, listDrawRect.left, listDrawRect.top, SRCCOPY);
    }

    // Blit header
    if (hasHeader) {
        CClientDC clientDC(this); //paint outside list area

        CRect headerClipRect = headerRect;
        CRgn headerClipRgn;
        headerClipRgn.CreateRectRgnIndirect(&headerClipRect);
        clientDC.SelectClipRgn(&headerClipRgn);
        clientDC.BitBlt(headerRect.left, headerRect.top, headerRect.Width(), headerRect.Height(), &m_headerBuffer.memDC, 0, 0, SRCCOPY);

        clientDC.SelectClipRgn(NULL);
    }
  } else {
    __super::OnPaint();
  }
}

void CMPCThemePlayerListCtrl::setAdditionalStyles(DWORD styles, bool exStyle /* = true*/)
{
    if (AppNeedsThemedControls()) {
        DWORD stylesToAdd = styles, stylesToRemove = 0;
        bool dummy;

        auto handleStyle = [&](DWORD style, bool& flag) {
          if (styles & style) {
            stylesToAdd &= ~style;
            stylesToRemove |= style;
            flag = true;
          }
        };


        if (styles & LVS_EX_FULLROWSELECT) {
            //we need these to remain, or else other columns may not get refreshed on a selection change.
            //no regressions observed yet, but unclear why we removed this style for custom draw previously
            //error was observed with playersubresyncbar
            //handleStyle(LVS_EX_FULLROWSELECT, fullRowSelect);
          fullRowSelect = true;
        }

        handleStyle(LVS_EX_GRIDLINES, themeGridLines);
        handleStyle(LVS_EX_DOUBLEBUFFER, dummy);
        handleStyle(WS_CLIPCHILDREN, clipChildWindows);

        if (exStyle) {
            SetExtendedStyle((GetExtendedStyle() | stylesToAdd) & ~stylesToRemove);
        } else {
            SetWindowLongPtrW(m_hWnd, GWL_STYLE, (GetStyle() | stylesToAdd) & ~stylesToRemove);
        }
    } else {
        if (exStyle) {
            SetExtendedStyle(GetExtendedStyle() | styles);
        } else {
            SetWindowLongPtrW(m_hWnd, GWL_STYLE, GetStyle() | styles);
        }
    }
}

void CMPCThemePlayerListCtrl::setHasCBImages(bool on)
{
    hasCBImages = on;
}

void CMPCThemePlayerListCtrl::setItemTextWithDefaultFlag(int nItem, int nSubItem, LPCTSTR lpszText, bool flagged)
{
    SetItemText(nItem, nSubItem, lpszText);
    setFlaggedItem(nItem, flagged);
}

void CMPCThemePlayerListCtrl::setFlaggedItem(int iItem, bool flagged)
{
    flaggedItems[iItem] = flagged;
}

bool CMPCThemePlayerListCtrl::getFlaggedItem(int iItem)
{
    auto it = flaggedItems.find(iItem);
    if (it != flaggedItems.end()) {
        return it->second;
    } else {
        return false;
    }
}

void CMPCThemePlayerListCtrl::DoDPIChanged()
{
    if (listMPCThemeFontBold.m_hObject) {
        listMPCThemeFontBold.DeleteObject();
    }

}


BOOL CMPCThemePlayerListCtrl::PreTranslateMessage(MSG* pMsg)
{
    if (AppNeedsThemedControls()) {
        if (!IsWindow(themedToolTip.m_hWnd)) {
            themedToolTip.Create(this, TTS_ALWAYSTIP);
            themedToolTip.enableFlickerHelper();
        }
        if (IsWindow(themedToolTip.m_hWnd)) {
            themedToolTip.RelayEvent(pMsg);
        }
    }
    return __super::PreTranslateMessage(pMsg);
}

void CMPCThemePlayerListCtrl::setCheckedColors(COLORREF checkedBG, COLORREF checkedText, COLORREF uncheckedText)
{
    checkedBGClr = checkedBG;
    checkedTextClr = checkedText;
    uncheckedTextClr = uncheckedText;
    hasCheckedColors = true;
}

void CMPCThemePlayerListCtrl::OnNcPaint()
{
    if (AppNeedsThemedControls()) {
        if (nullptr != themedSBHelper) {
            themedSBHelper->themedNcPaintWithSB();
        } else {
            CMPCThemeScrollBarHelper::themedNcPaint(this, this);
        }
    } else {
        __super::OnNcPaint();
    }
}


int CMPCThemePlayerListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    if (AppNeedsThemedControls()) {
        SetBkColor(CMPCTheme::ContentBGColor);
        subclassHeader();
    }

    return 0;
}

BOOL CMPCThemePlayerListCtrl::OnLvnEndScroll(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (AppNeedsThemedControls()) {
        if (nullptr != themedSBHelper) {
            themedSBHelper->updateScrollInfo();
        }
        *pResult = 0;
    }
    return FALSE;
}

void CMPCThemePlayerListCtrl::updateSB()
{
    if (nullptr != themedSBHelper) {
        themedSBHelper->hideNativeScrollBars();
    }
}

void CMPCThemePlayerListCtrl::updateScrollInfo(bool invalidate /*=false*/)
{
    if (nullptr != themedSBHelper) {
        themedSBHelper->updateScrollInfo(invalidate);
    }
}

LRESULT CMPCThemePlayerListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (AppNeedsThemedControls() && nullptr != themedSBHelper) {
        if (themedSBHelper->WindowProc(this, message, wParam, lParam)) {
            return 1;
        }
    }

    if (message == WM_NOTIFY) {
        LPNMHDR pNMHDR = (LPNMHDR)lParam;

        // Check if it's a message from our header control
        CHeaderCtrl* pHeader = GetHeaderCtrl();
        if (pHeader && pNMHDR->hwndFrom == pHeader->GetSafeHwnd()) {
            // Handle header notifications that might cause repainting
            switch (pNMHDR->code) {
            case HDN_BEGINTRACK:
            case HDN_ENDTRACK:
                pHeader->SendMessageW(WM_NOTIFY, pNMHDR->code, (LPARAM)pNMHDR);
            case HDN_TRACK:
            case HDN_DIVIDERDBLCLICK:
                // Column is being resized - invalidate our buffer
                m_listBuffer.bValid = false;
                m_headerBuffer.bValid = false;
                Invalidate(FALSE);
                break;
            }
        }
    }

    return __super::WindowProc(message, wParam, lParam);
}

void CMPCThemePlayerListCtrl::updateToolTip(CPoint point)
{
    if (AppNeedsThemedControls() && nullptr != themedToolTip) {
        TOOLINFO ti = { 0 };
        UINT_PTR tid = OnToolHitTest(point, &ti);
        //OnToolHitTest returns -1 on failure but doesn't update uId to match

        if (tid == -1 || themedToolTipCid != ti.uId) { //if no tooltip, or id has changed, remove old tool
            if (themedToolTip.GetToolCount() > 0) {
                themedToolTip.DelTool(this);
                themedToolTip.Activate(FALSE);
            }
            themedToolTipCid = (UINT_PTR) - 1;
        }

        if (tid != -1 && themedToolTipCid != ti.uId && 0 != ti.uId) {

            themedToolTipCid = ti.uId;

            CRect cr;
            GetClientRect(&cr); //we reset the tooltip every time we move anyway, so this rect is adequate

            themedToolTip.AddTool(this, LPSTR_TEXTCALLBACK, &cr, ti.uId);
            themedToolTip.Activate(TRUE);
        }
    }
}

void CMPCThemePlayerListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    __super::OnMouseMove(nFlags, point);
    updateToolTip(point);
}


BOOL CMPCThemePlayerListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    BOOL ret = __super::OnMouseWheel(nFlags, zDelta, pt);
    ScreenToClient(&pt);
    updateToolTip(pt);
    return ret;
}


void CMPCThemePlayerListCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    __super::OnNcCalcSize(bCalcValidRects, lpncsp);
    if (AppNeedsThemedControls()) {
        if (GetStyle() & WS_HSCROLL && nullptr == themedSBHelper) {
            themedSBHelper = DEBUG_NEW CMPCThemeScrollBarHelper(this);
        }
        ::PostMessage(m_hWnd, PLAYER_PLAYLIST_UPDATE_SCROLLBAR, (WPARAM)0, (LPARAM)TRUE);
    }
}

void CMPCThemePlayerListCtrl::drawItem(CDC* pDC, int nItem, int nSubItem)
{
    if (IsItemVisible(nItem)) {

        CRect rect, rRow, rIcon, rText, rTextBG, rectDC, rClient;
        GetClientRect(rClient);
        GetItemRect(nItem, rRow, LVIR_BOUNDS);
        GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rText);
        GetSubItemRect(nItem, nSubItem, LVIR_ICON, rIcon);
        GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
        DWORD dwStyle = GetStyle() & LVS_TYPEMASK;

        if (0 == nSubItem) { //getsubitemrect gives whole row for 0/LVIR_BOUNDS.  but LVIR_LABEL is limited to text bounds.  MSDN undocumented behavior
            rect.right = rText.right;
        }

        CFont* curFont = pDC->GetCurrentFont();

        //issubitemvisible
        if (rClient.left <= rect.right && rClient.right >= rect.left && rClient.top <= rect.bottom && rClient.bottom >= rect.top) {
            COLORREF textColor = CMPCTheme::TextFGColor;
            COLORREF bgColor = CMPCTheme::ContentBGColor;
            COLORREF selectedBGColor = CMPCTheme::ContentSelectedColor;

            COLORREF oldTextColor = pDC->GetTextColor();
            COLORREF oldBkColor = pDC->GetBkColor();

            CString text = GetItemText(nItem, nSubItem);
            if (nullptr != customThemeInterface) { //subclasses can override colors here
                bool overrideSelectedBG = false;
                customThemeInterface->GetCustomTextColors(nItem, nSubItem, textColor, bgColor, overrideSelectedBG);
                if (overrideSelectedBG) {
                    selectedBGColor = bgColor;
                }
            }

            pDC->SetTextColor(textColor);
            pDC->SetBkColor(bgColor);

            rectDC = rRow;

            if (!IsWindowEnabled() && 0 == nSubItem) { //no gridlines, bg for full row
                pDC->FillSolidRect(rRow, CMPCTheme::ListCtrlDisabledBGColor);
            } else {
                pDC->FillSolidRect(rect, CMPCTheme::ContentBGColor); //no flicker because we have a memory dc
            }

            rTextBG = rText;
            CHeaderCtrl* hdrCtrl = GetHeaderCtrl();
            int align = DT_LEFT;
            if (nullptr != hdrCtrl) {
                HDITEM hditem = { 0 };
                hditem.mask = HDI_FORMAT;
                hdrCtrl->GetItem(nSubItem, &hditem);
                align = hditem.fmt & HDF_JUSTIFYMASK;
            }
            UINT textFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
            if (align == HDF_CENTER) {
                textFormat |= DT_CENTER;
            } else if (align == HDF_LEFT) {
                textFormat |= DT_LEFT;
                if (nSubItem == 0) {//less indent for first column
                    rText.left += 2;
                } else {
                    rText.left += 6;
                }
            } else {
                textFormat |= DT_RIGHT;
                rText.right -= 6;
            }

            bool isChecked = false;
            int contentLeft = rText.left;
            if (rIcon.Width() > 0) {
                LVITEM lvi = { 0 };
                lvi.iItem = nItem;
                lvi.iSubItem = 0;
                lvi.mask = LVIF_IMAGE;
                GetItem(&lvi);

                if (nSubItem == 0) {
                    contentLeft = rIcon.left;
                    if (hasCBImages) { //draw manually to match theme
                        rIcon.DeflateRect(0, 0, 1, 0);
                        if (rIcon.Height() > rIcon.Width()) {
                            rIcon.DeflateRect(0, (rIcon.Height() - rIcon.Width()) / 2); //as tall as wide
                        }

                        CMPCThemeUtil::drawCheckBox(GetParent(), lvi.iImage, false, false, rIcon, pDC);
                    } else {
                        if (dwStyle == LVS_ICON) {
                        } else if (dwStyle == LVS_SMALLICON || dwStyle == LVS_LIST || dwStyle == LVS_REPORT) {
                            CImageList* ilist = GetImageList(LVSIL_SMALL);
                            int cx, cy;
                            ImageList_GetIconSize(ilist->m_hImageList, &cx, &cy);
                            rIcon.top += (rIcon.Height() - cy) / 2;
                            ilist->Draw(pDC, lvi.iImage, rIcon.TopLeft(), ILD_TRANSPARENT);
                        }
                    }
                    if (align == HDF_LEFT) {
                        rText.left += 2;    //more ident after image
                    }
                }
            }
            if (0 != (GetExtendedStyle() & LVS_EX_CHECKBOXES) && INDEXTOSTATEIMAGEMASK(0) != GetItemState(nItem, LVIS_STATEIMAGEMASK)) {
                isChecked = (TRUE == GetCheck(nItem));
                if (nSubItem == 0) {
                    int cbSize = GetSystemMetrics(SM_CXMENUCHECK);
                    int cbYMargin = (rect.Height() - cbSize - 1) / 2;
                    int cbXMargin = (contentLeft - rect.left - cbSize) / 2;
                    CRect rcb = { rect.left + cbXMargin, rect.top + cbYMargin, rect.left + cbXMargin + cbSize, rect.top + cbYMargin + cbSize };
                    CMPCThemeUtil::drawCheckBox(GetParent(), isChecked, false, true, rcb, pDC);
                }
            }

            if (IsWindowEnabled()) {
                bool selected = false;
                if (GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED && (nSubItem == 0 || fullRowSelect) && (GetStyle() & LVS_SHOWSELALWAYS || GetFocus() == this)) {
                    bgColor = selectedBGColor;
                    if (LVS_REPORT != dwStyle) { //in list mode we don't fill the "whole" column
                        CRect tmp = rText;
                        pDC->DrawTextW(text, tmp, textFormat | DT_CALCRECT); //end of string
                        rTextBG.right = tmp.right + (rText.left - rTextBG.left); //end of string plus same indent from the left side
                    }
                    selected = true;
                } else if (hasCheckedColors) {
                    if (isChecked && checkedBGClr != -1) {
                        bgColor = checkedBGClr;
                    }
                    if (isChecked && checkedTextClr != -1) {
                        pDC->SetTextColor(checkedTextClr);
                    }
                    if (!isChecked && uncheckedTextClr != -1) {
                        pDC->SetTextColor(uncheckedTextClr);
                    }
                }
                pDC->FillSolidRect(rTextBG, bgColor);

                if (themeGridLines || (nullptr != customThemeInterface && customThemeInterface->UseCustomGrid())) {
                    CRect rGrid = rect;
                    rGrid.bottom -= 1;
                    CPen gridPenV, gridPenH, *oldPen;
                    if (nullptr != customThemeInterface && customThemeInterface->UseCustomGrid()) {
                        COLORREF horzGridColor, vertGridColor;
                        customThemeInterface->GetCustomGridColors(nItem, horzGridColor, vertGridColor);
                        gridPenV.CreatePen(PS_SOLID, 1, vertGridColor);
                        gridPenH.CreatePen(PS_SOLID, 1, horzGridColor);
                    } else {
                        gridPenV.CreatePen(PS_SOLID, 1, CMPCTheme::ListCtrlGridColor);
                        gridPenH.CreatePen(PS_SOLID, 1, CMPCTheme::ListCtrlGridColor);
                    }

                    oldPen = pDC->SelectObject(&gridPenV);
                    if (nSubItem != 0) {
                        pDC->MoveTo(rGrid.TopLeft());
                        pDC->LineTo(rGrid.left, rGrid.bottom);
                    } else {
                        pDC->MoveTo(rGrid.left, rGrid.bottom);
                    }

                    pDC->SelectObject(&gridPenH);
                    pDC->LineTo(rGrid.BottomRight());

                    pDC->SelectObject(&gridPenV);
                    pDC->LineTo(rGrid.right, rGrid.top);

                    pDC->SelectObject(oldPen);
                    gridPenV.DeleteObject();
                    gridPenH.DeleteObject();
                } else if (selected) {
                    CBrush borderBG;
                    borderBG.CreateSolidBrush(CMPCTheme::ListCtrlDisabledBGColor);
                    pDC->FrameRect(rTextBG, &borderBG);
                    borderBG.DeleteObject();
                }
            }

            if (getFlaggedItem(nItem)) { //could be a setting, but flagged items are bold for now
                if (!listMPCThemeFontBold.m_hObject) {
                    listMPCThemeFont = GetFont();
                    LOGFONT lf;
                    listMPCThemeFont->GetLogFont(&lf);
                    lf.lfWeight = FW_BOLD;
                    listMPCThemeFontBold.CreateFontIndirect(&lf);
                }

                pDC->SelectObject(listMPCThemeFontBold);
            }
            pDC->DrawTextW(text, rText, textFormat);
            pDC->SetTextColor(oldTextColor);
            pDC->SetBkColor(oldBkColor);
            pDC->SelectObject(curFont);
        }
    }
}

BOOL CMPCThemePlayerListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
#if 0
    if (AppNeedsThemedControls()) {
        NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

        *pResult = CDRF_DODEFAULT;
        if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT) {
            if (nullptr != customThemeInterface) {
                customThemeInterface->DoCustomPrePaint();
            }
            *pResult = CDRF_NOTIFYITEMDRAW;
        } else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
            DWORD dwStyle = GetStyle() & LVS_TYPEMASK;
            if (LVS_REPORT == dwStyle) {
                *pResult = CDRF_NOTIFYSUBITEMDRAW;
            } else {
                int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
                CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
                drawItem(pDC, nItem, 0);
                *pResult = CDRF_SKIPDEFAULT;
            }
        } else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM)) {
            if (GetStyle() & LVS_OWNERDRAWFIXED) {
                //found that for ownerdraw routines, we can end up here and draw both ways on hover/tooltip. this should prevent it
                *pResult = CDRF_DODEFAULT;
            } else {
                int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
                if (IsItemVisible(nItem)) {
                    int nSubItem = pLVCD->iSubItem;
                    CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
                    drawItem(pDC, nItem, nSubItem);
                }
                *pResult = CDRF_SKIPDEFAULT;
            }
        }
        return TRUE;
    }
#endif
    return FALSE;
}


BOOL CMPCThemePlayerListCtrl::OnEraseBkgnd(CDC* pDC) {
    if (PaintHooksActive()) {
        EraseBkgnd(pDC);
    }

    return TRUE;
}


BOOL CMPCThemePlayerListCtrl::EraseBkgnd(CDC* pDC)
{
    if (AppNeedsThemedControls()) {
        CRect r;
        GetClientRect(r);
        int dcState = pDC->SaveDC();
        for (int y = 0; y < GetItemCount(); y++) {
            CRect clip;
            GetItemRect(y, clip, LVIR_BOUNDS);
            pDC->ExcludeClipRect(clip);
        }
        pDC->FillSolidRect(r, CMPCTheme::ContentBGColor);

        if (themeGridLines || (nullptr != customThemeInterface && customThemeInterface->UseCustomGrid())) {

            CPen gridPen, *oldPen;
            gridPen.CreatePen(PS_SOLID, 1, CMPCTheme::ListCtrlGridColor);
            oldPen = pDC->SelectObject(&gridPen);

            if (GetItemCount() > 0) {
                CRect gr;
                for (int x = 0; x < themedHdrCtrl.GetItemCount(); x++) {
                    themedHdrCtrl.GetItemRect(x, gr);
                    pDC->MoveTo(gr.right, r.top);
                    pDC->LineTo(gr.right, r.bottom);
                }
                gr.bottom = 0;
                for (int y = 0; y < GetItemCount() || gr.bottom < r.bottom; y++) {
                    if (y >= GetItemCount()) {
                        gr.OffsetRect(0, gr.Height());
                    } else {
                        GetItemRect(y, gr, LVIR_BOUNDS);
                    }
                    {
                        CPen horzPen;
                        pDC->MoveTo(r.left, gr.bottom - 1);
                        if (nullptr != customThemeInterface && customThemeInterface->UseCustomGrid()) {
                            COLORREF horzGridColor, tmp;
                            customThemeInterface->GetCustomGridColors(y, horzGridColor, tmp);
                            horzPen.CreatePen(PS_SOLID, 1, horzGridColor);
                            pDC->SelectObject(&horzPen);
                            pDC->LineTo(r.right, gr.bottom - 1);
                            pDC->SelectObject(&gridPen);
                            horzPen.DeleteObject();
                        } else {
                            pDC->LineTo(r.right, gr.bottom - 1);
                        }
                    }
                }
            }
            pDC->SelectObject(oldPen);
            gridPen.DeleteObject();
        }
        pDC->RestoreDC(dcState);
    } else {
        return __super::OnEraseBkgnd(pDC);
    }
    return TRUE;
}


HBRUSH CMPCThemePlayerListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH ret;
    ret = getCtlColor(pDC, pWnd, nCtlColor);
    if (nullptr != ret) {
        return ret;
    } else {
        return __super::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}


BOOL CMPCThemePlayerListCtrl::OnHdnEndtrack(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
    //    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    if (AppNeedsThemedControls()) {
        if (nullptr != themedSBHelper) {
            themedSBHelper->updateScrollInfo();
        }
    }
    *pResult = 0;

    //we don't want to prevent this event from being processed
    //it's used when "show windows contents while dragging" is false,
    //to draw the outline of the resized column
    return FALSE; 
}

LRESULT CMPCThemePlayerListCtrl::OnDelayed_UpdateScrollbar(WPARAM, LPARAM invalidate)
{
    updateScrollInfo((bool)invalidate);
    return 0;
}

BOOL CMPCThemePlayerListCtrl::OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    //LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    if (AppNeedsThemedControls()) {
        ::PostMessage(m_hWnd, PLAYER_PLAYLIST_UPDATE_SCROLLBAR, (WPARAM)0, (LPARAM)0);
    }
    *pResult = 0;
    return FALSE;
}

void CMPCThemePlayerListCtrl::DrawAllItems(CDC* pDC, const CRect& drawRect) {
    int itemCount = GetItemCount();
    if (itemCount == 0) return;

    DWORD style = GetStyle() & LVS_TYPEMASK;

    if (style == LVS_REPORT) {
        int topIndex = GetTopIndex();
        int itemCount = GetItemCount();
        int visibleCount = GetCountPerPage();

        // Calculate item range to draw (with some buffer)
        int startItem = std::max(0, topIndex - 1);
        int endItem = std::min(itemCount - 1, topIndex + visibleCount + 1);

        // Get header control to determine number of columns
        CHeaderCtrl* pHeader = GetHeaderCtrl();
        int colCount = pHeader ? pHeader->GetItemCount() : 1;

        // Loop through all visible items
        for (int nItem = startItem; nItem <= endItem; nItem++) {
            CRect itemRect;
            GetItemRect(nItem, &itemRect, LVIR_BOUNDS);

            // Check if this item intersects with the draw rectangle
            CRect intersectRect;
            if (!intersectRect.IntersectRect(&itemRect, &drawRect))
                continue;

            // Loop through all subitems (columns) for this item
            for (int nSubItem = 0; nSubItem < colCount; nSubItem++) {
                // Call the custom draw function for each item/subitem combination
                drawItem(pDC, nItem, nSubItem);
            }
        }
    } else {
        for (int nItem = 0; nItem < itemCount; nItem++) {
            CRect itemRect;
            GetItemRect(nItem, &itemRect, LVIR_BOUNDS);

            // Check if this item intersects with the draw rectangle
            CRect intersectRect;
            if (intersectRect.IntersectRect(&itemRect, &drawRect)) {
                // Non-report views only have one "column" (no subitems)
                drawItem(pDC, nItem, 0);
            }
        }
    }
}
