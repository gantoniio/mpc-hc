#pragma once
#include "CMPCThemeScrollBar.h"
#include <mutex>
class CMPCThemeTreeCtrl;

class CMPCThemeScrollable
{
public:
    CMPCThemeScrollable() {};
    ~CMPCThemeScrollable() {};
    virtual void doDefault() {};
};

class ScrollBarHelperInfo {
public:
    ScrollBarHelperInfo(CWnd* w);
    bool UpdateHelperInfo(CWnd* w);
    bool operator==(ScrollBarHelperInfo& lhs);
    bool operator!=(ScrollBarHelperInfo& lhs) { return !operator==(lhs); }
    CRect wr, corner, wrOnParent;
    CPoint clientOffset;
    int sbThickness;
    int borderThickness;
    bool canVSB;
    bool canHSB;
    bool needsSBCorner;
};

class CMPCThemeScrollBarHelper
{
protected:
    CWnd* window, *pParent;
    CMPCThemeScrollBar vertSB, horzSB;
    ScrollBarHelperInfo helperInfo;
    std::recursive_mutex helperMutex;
    bool setWindowRegionActive;
    static void doNcPaint(CWnd* window);
public:
    CMPCThemeScrollBarHelper(CWnd* scrollWindow);
    ~CMPCThemeScrollBarHelper();
    void createThemedScrollBars();
    void OnWindowPosChanged();
    void setWindowRegionExclusive(HRGN h);
    void hideNativeScrollBars();
    void updateScrollInfo(bool invalidate = false);
    bool WindowProc(CListCtrl* list, UINT message, WPARAM wParam, LPARAM lParam);
    bool WindowProc(CTreeCtrl* tree, UINT message, WPARAM wParam, LPARAM lParam);
    void themedNcPaintWithSB();
    static void themedNcPaint(CWnd* window, CMPCThemeScrollable* swindow);
};

