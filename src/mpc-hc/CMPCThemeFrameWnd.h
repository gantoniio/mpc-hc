#pragma once
#include <afxwin.h>
#include "CMPCTheme.h"
#include "CMPCThemeFrameUtil.h"
#include "CMPCThemeTitleBarControlButton.h"

class CMPCThemeFrameWnd :
	public CFrameWnd,
    public CMPCThemeFrameUtil
{
public:
	CMPCThemeFrameWnd();
protected: 
	DECLARE_DYNAMIC(CMPCThemeFrameWnd)
public:
    virtual ~CMPCThemeFrameWnd();
    virtual void RecalcLayout(BOOL bNotify = TRUE);
    CRect getTitleBarRect();
    CRect getSysMenuIconRect();
protected:
	CRect   borders;
	int     titlebarHeight;
    void recalcTitleBar();
    CMPCThemeTitleBarControlButton minimizeButton, maximizeButton, closeButton;
    void GetIconRects(CRect titlebarRect, CRect& closeRect, CRect& maximizeRect, CRect& minimizeRect);
    void checkFrame(LONG style);
    void recalcFrame();
    void allowMPCThemeFrameAdjust(bool allow);
    enum frameState {
        frameNormal,
        frameThemed,
    };
private:
    TITLEBARINFO titleBarInfo;
    frameState currentFrameState, wantFrameState;
    bool frameChanged, disableFrameAdjust;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcMouseLeave();
};

