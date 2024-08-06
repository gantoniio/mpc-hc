/*
 * (C) 2021-2023 see Authors.txt
 *
 * This file is part of MPC-BE.
 *
 * MPC-BE is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-BE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <afxwin.h>
#include "CMPCThemeResizableDialog.h"
#include "CMPCThemeEdit.h"
#include "CMPCThemeButton.h"
#include "CMPCThemePlayerListCtrl.h"

// CAddCommandDlg dialog

class CAddCommandDlg : public CMPCThemeResizableDialog
{
	//DECLARE_DYNAMIC(CAddCommandDlg)

private:
	CMPCThemeEdit m_FilterEdit;
	CMPCThemePlayerListCtrl m_list;
	CMPCThemeButton m_okButton;

	enum {
		COL_CMD,
		COL_ID,
		COL_COUNT
	};

	WORD m_selectedID = 0;
	UINT_PTR m_nFilterTimerID = 0;

	void FillList();
	void FilterList();

public:
	CAddCommandDlg(CWnd* pParent = nullptr);
	virtual ~CAddCommandDlg();

	enum { IDD = IDD_ADDCOMMAND_DLG };

	WORD GetSelectedCommandID() { return m_selectedID; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeFilterEdit();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
};
