/*
* (C) 2014-2017 see Authors.txt
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
#include "PPageToolBar.h"
#include "mplayerc.h"
#include "MainFrm.h"


IMPLEMENT_DYNAMIC(CPPageToolBar, CMPCThemePPageBase)

CPPageToolBar::CPPageToolBar()
    : CMPCThemePPageBase(IDD, IDD)

{
}

void CPPageToolBar::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_list_active);
    DDX_Control(pDX, IDC_LIST2, m_list_inactive);
    DDX_Control(pDX, IDC_BUTTON3, leftButton);
    DDX_Control(pDX, IDC_BUTTON4, rightButton);
    DDX_Control(pDX, IDC_BUTTON5, upButton);
    DDX_Control(pDX, IDC_BUTTON6, downButton);
}

void CPPageToolBar::LoadToolBarButtons() {
    m_list_active.DeleteAllItems();
    m_list_inactive.DeleteAllItems();

    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    CToolBarCtrl& tbctrl = tb.GetToolBarCtrl();

    auto supportedButtons = tb.GetSupportedSvgButtons();
    std::set<int> idsAdded;
    bool foundFirst = false;
    for (int i = 0; i < tbctrl.GetButtonCount(); i++) {
        TBBUTTON button;
        tbctrl.GetButton(i, &button);
        if (button.fsStyle != TBBS_SEPARATOR && supportedButtons.count(button.idCommand)) {
            int index = m_list_active.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, i, tb.GetStringFromID(button.idCommand), 0, 0, button.iBitmap, button.idCommand);
            auto& buttonInfo = supportedButtons[button.idCommand];

            if (!buttonInfo.positionLocked && !foundFirst) {
                m_list_active.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
                m_list_active.SetSelectionMark(index);
                foundFirst = true;
            }
            idsAdded.insert(button.idCommand);
        }
    }

    int i = 0;
    for (auto& it : supportedButtons) {
        if (0 == idsAdded.count(it.first) && it.second.style != TBBS_SEPARATOR) {
            int index = m_list_inactive.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, i++, tb.GetStringFromID(it.first), 0, 0, it.second.svgIndex, it.first);
            if (index == 0) {
                m_list_inactive.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
                m_list_inactive.SetSelectionMark(index);
            }
        }
    }
}

BOOL CPPageToolBar::OnInitDialog()
{
    __super::OnInitDialog();

    CRect brc;
    leftButton.GetWindowRect(brc);
    leftButton.SetWindowTextW(L"");
    rightButton.SetWindowTextW(L"");
    upButton.SetWindowTextW(L"");
    downButton.SetWindowTextW(L"");
    SetMPCThemeButtonIcon(IDC_BUTTON3, { IDF_SVG_ARROW, int(0.7f * brc.Width()), TRANSFORM_FLIP_HORZ });
    SetMPCThemeButtonIcon(IDC_BUTTON4, { IDF_SVG_ARROW, int(0.7f * brc.Width()) });
    SetMPCThemeButtonIcon(IDC_BUTTON5, { IDF_SVG_ARROW, int(0.7f * brc.Width()), TRANSFORM_ROT90CCW });
    SetMPCThemeButtonIcon(IDC_BUTTON6, { IDF_SVG_ARROW, int(0.7f * brc.Width()), TRANSFORM_ROT90CW });

    SetRedraw(FALSE);

    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    CToolBarCtrl& tbctrl = tb.GetToolBarCtrl();

    m_list_active.SetExtendedStyle(m_list_active.GetExtendedStyle() /* | LVS_EX_FULLROWSELECT */ /*| LVS_EX_DOUBLEBUFFER */ | LVS_EX_INFOTIP | LVS_ICON | LVS_SHAREIMAGELISTS);
    m_list_active.setAdditionalStyles(LVS_EX_FULLROWSELECT);
    m_list_active.InsertColumn(COL_IMAGE, L"", LVCFMT_LEFT);
    m_list_active.SetColumnWidth(COL_IMAGE, LVSCW_AUTOSIZE_USEHEADER);
    m_list_active.SetImageList(tbctrl.GetImageList(), LVSIL_SMALL);
    m_list_active.setColorInterface(this);

    m_list_inactive.SetExtendedStyle(m_list_inactive.GetExtendedStyle() /* | LVS_EX_FULLROWSELECT */ /*| LVS_EX_DOUBLEBUFFER */ | LVS_EX_INFOTIP | LVS_ICON | LVS_SHAREIMAGELISTS);
    m_list_inactive.setAdditionalStyles(LVS_EX_FULLROWSELECT);
    m_list_inactive.InsertColumn(COL_IMAGE, L"", LVCFMT_LEFT);
    m_list_inactive.SetColumnWidth(COL_IMAGE, LVSCW_AUTOSIZE_USEHEADER);
    m_list_inactive.SetImageList(tbctrl.GetImageList(), LVSIL_SMALL);

    LoadToolBarButtons();

    SetRedraw(TRUE);
    return TRUE;
}

BOOL CPPageToolBar::OnApply()
{
    auto& s = AfxGetAppSettings();
    return __super::OnApply();
}

BEGIN_MESSAGE_MAP(CPPageToolBar, CMPCThemePPageBase)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnCustomdrawList)
    ON_BN_CLICKED(IDC_BUTTON3, &CPPageToolBar::MoveLeft)
    ON_BN_CLICKED(IDC_BUTTON4, &CPPageToolBar::MoveRight)
    ON_BN_CLICKED(IDC_BUTTON5, &CPPageToolBar::MoveUp)
    ON_BN_CLICKED(IDC_BUTTON6, &CPPageToolBar::MoveDown)
    ON_BN_CLICKED(IDC_BUTTON1, &CPPageToolBar::DefaultButtons)
END_MESSAGE_MAP()

bool CPPageToolBar::InsertButton(int beforeID, int buttonID) {
    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    CToolBarCtrl& tbctrl = tb.GetToolBarCtrl();

    for (int i = 0; i < tbctrl.GetButtonCount(); i++) {
        TBBUTTON tButton;
        tbctrl.GetButton(i, &tButton);
        if (tButton.idCommand == beforeID) {
            if (beforeID == ID_VOLUME_MUTE) {
                i -= 1; //this is to force inserting before the hidden spacer
            }
            TBBUTTON button = tb.GetStandardButton(buttonID);
            tbctrl.InsertButton(i, &button);
            tb.ToolbarChange();
            return true;
        }
    }
    return false;
}

bool CPPageToolBar::DeleteButton(int buttonID) {
    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    CToolBarCtrl& tbctrl = tb.GetToolBarCtrl();

    for (int i = 0; i < tbctrl.GetButtonCount(); i++) {
        TBBUTTON tButton;
        tbctrl.GetButton(i, &tButton);
        if (tButton.idCommand == buttonID) {
            tbctrl.DeleteButton(i);
            tb.ToolbarChange();
            return true;
        }
    }
    return false;
}

bool CPPageToolBar::IsValidInsertPos(int destRow) {
    if (destRow == -1 || destRow >= m_list_active.GetItemCount()) {
        return false;
    }
    //avoid inserting between two locked rows, or before first locked or after last locked

    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    auto supportedButtons = tb.GetSupportedSvgButtons();

    int tidCommand = (int)m_list_active.GetItemData(destRow);
    if (supportedButtons.count(tidCommand) == 0) { //this should never happen
        return false;
    }
    if (destRow == 0) { //first
        if (supportedButtons[tidCommand].positionLocked) {//can't insert before locked first button
            return false;
        }
    } else if (destRow == m_list_active.GetItemCount() - 1) { //last--don't insert before if prior button locked
        int priorIdCommand = (int)m_list_active.GetItemData(destRow - 1);
        if (supportedButtons.count(priorIdCommand) == 0 || supportedButtons[priorIdCommand].positionLocked) {
            return false;
        }
    } else {//we have at least two rows.  if the insert point is between two locked rows we will not allow it
        if (supportedButtons[tidCommand].positionLocked) {
            int priorIdCommand = (int)m_list_active.GetItemData(destRow - 1);
            if (supportedButtons.count(priorIdCommand) == 0 || supportedButtons[priorIdCommand].positionLocked) {
                return false;
            }
        }
    }
}

bool CPPageToolBar::MoveButton(CMPCThemePlayerListCtrl& srcList, CMPCThemePlayerListCtrl& dstList) {
    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    CToolBarCtrl& tbctrl = tb.GetToolBarCtrl();
    auto supportedButtons = tb.GetSupportedSvgButtons();

    bool addingButton = (dstList == m_list_active);

    int selectedRow = srcList.GetSelectionMark(); 
    if (selectedRow == -1) {
        return false;
    }
    int buttonID = (int)srcList.GetItemData(selectedRow);
    int beforeID = -1;
    if (supportedButtons.count(buttonID) == 0 || supportedButtons[buttonID].positionLocked) {
        return false;
    }

    int destRow = dstList.GetSelectionMark();

    if (!addingButton) {
        if (destRow == -1) {
            destRow = 0;
        }
    } else { //active
        if (destRow == -1) {
            for (destRow = 0; destRow < dstList.GetItemCount(); destRow++) {
                beforeID = (int)dstList.GetItemData(destRow);
                if (supportedButtons.count(beforeID) != 0 && !supportedButtons[beforeID].positionLocked) {
                    break;
                }
            }
        }
        if (!IsValidInsertPos(destRow)) {
            return false;
        }
        beforeID = (int)dstList.GetItemData(destRow);
    }

    dstList.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, destRow, tb.GetStringFromID(buttonID), 0, 0, supportedButtons[buttonID].svgIndex, buttonID);
    srcList.DeleteItem(selectedRow);

    if (addingButton) {
        InsertButton(beforeID, buttonID);
    } else {
        DeleteButton(buttonID);
    }

    return true;
}

void CPPageToolBar::MoveLeft() {
    MoveButton(m_list_inactive, m_list_active);
}

void CPPageToolBar::MoveRight() {
    MoveButton(m_list_active, m_list_inactive);
}

bool CPPageToolBar::OrderButton(ButtonPosition pos) {
    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    CToolBarCtrl& tbctrl = tb.GetToolBarCtrl();
    auto supportedButtons = tb.GetSupportedSvgButtons();

    int selectedRow = m_list_active.GetSelectionMark();
    if (selectedRow == -1) {
        return false;
    }
    int buttonID = (int)m_list_active.GetItemData(selectedRow);
    if (supportedButtons.count(buttonID) == 0 || supportedButtons[buttonID].positionLocked) {
        return false;
    }

    if (pos == PREVIOUS_POSITION) {
        for (int insertRow = selectedRow - 1; insertRow > -1; insertRow--) {
            if (IsValidInsertPos(insertRow)) {
                int beforeID = (int)m_list_active.GetItemData(insertRow);

                m_list_active.DeleteItem(selectedRow);
                m_list_active.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, insertRow, tb.GetStringFromID(buttonID), 0, 0, supportedButtons[buttonID].svgIndex, buttonID);

                m_list_active.SetItemState(insertRow, LVIS_SELECTED, LVIS_SELECTED);
                m_list_active.SetSelectionMark(insertRow);

                DeleteButton(buttonID);
                InsertButton(beforeID, buttonID);
                break;
            }
        }
    } else {
        for (int insertRow = selectedRow + 2; insertRow < m_list_active.GetItemCount(); insertRow++) {
            if (IsValidInsertPos(insertRow)) {
                int beforeID = (int)m_list_active.GetItemData(insertRow);

                m_list_active.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, insertRow, tb.GetStringFromID(buttonID), 0, 0, supportedButtons[buttonID].svgIndex, buttonID);

                m_list_active.SetItemState(insertRow, LVIS_SELECTED, LVIS_SELECTED);
                m_list_active.SetSelectionMark(insertRow);

                m_list_active.DeleteItem(selectedRow);

                DeleteButton(buttonID);
                InsertButton(beforeID, buttonID);
                break;
            }
        }
    }

    return true;
}

void CPPageToolBar::MoveUp() {
    OrderButton(PREVIOUS_POSITION);
}


void CPPageToolBar::MoveDown() {
    OrderButton(NEXT_POSITION);
}

void CPPageToolBar::GetCustomTextColors(INT_PTR nItem, int iSubItem, COLORREF& clrText, COLORREF& clrTextBk, bool& overrideSelectedBG) {
    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    CToolBarCtrl& tbctrl = tb.GetToolBarCtrl();
    auto supportedButtons = tb.GetSupportedSvgButtons();

    int buttonID = (int)m_list_active.GetItemData(nItem);
    int beforeID = -1;
    if (supportedButtons.count(buttonID) == 0 || supportedButtons[buttonID].positionLocked) {
        overrideSelectedBG = true;
        if (AppNeedsThemedControls()) {
            clrText = CMPCTheme::ContentTextDisabledFGColorFade2;
            clrTextBk = CMPCTheme::ContentBGColor;
        } else {
            clrText = RGB(224, 224, 224);
            clrTextBk = RGB(255, 255, 255);
        }
    }
}

void CPPageToolBar::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult) {
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    *pResult = CDRF_DODEFAULT;

    if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage) {
        *pResult = CDRF_NOTIFYITEMDRAW;
    } else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage) {
        bool overrideSelectedBG = false;
        GetCustomTextColors(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, pLVCD->clrText, pLVCD->clrTextBk, overrideSelectedBG);
        if (overrideSelectedBG) {
            pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
        }
    }
}


void CPPageToolBar::DefaultButtons() {
    CPlayerToolBar& tb = AfxGetMainFrame()->m_wndToolBar;
    tb.ToolBarReset();
    LoadToolBarButtons();
}
