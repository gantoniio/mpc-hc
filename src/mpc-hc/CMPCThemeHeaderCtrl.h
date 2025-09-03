#pragma once
#include <afxcmn.h>

class CMPCThemePlayerListCtrl;

class CMPCThemeHeaderCtrl :
    public CHeaderCtrl
{
protected:
    int hotItem;
    CMPCThemePlayerListCtrl* parent;
    void checkHot(CPoint point);
    void drawSortArrow(CDC* dc, COLORREF arrowClr, CRect arrowRect, bool ascending);
    void drawItem(int nItem, CRect rText, CDC* pDC);
public:
    CMPCThemeHeaderCtrl();
    virtual ~CMPCThemeHeaderCtrl();
    void DrawAllItems(CDC* pDC, CPoint offset);
    void SetParent(CMPCThemePlayerListCtrl* parent) { this->parent = parent; }

    DECLARE_MESSAGE_MAP()
    afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHdnTrack(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

