#pragma once
#include "CMPCThemeFrameUtil.h"

class CMPCThemeTitleBarControlButton :
    public CMFCButton {
public:
    CMPCThemeTitleBarControlButton(LRESULT buttonType);
    void setParentFrame(CMPCThemeFrameUtil* parent);
protected:
    void drawTitleBarButton(CDC* pDC, CRect iconRect, std::vector<CMPCTheme::pathPoint> icon, double dpiScaling, bool antiAlias = false);
    const std::vector<CMPCTheme::pathPoint>& getIconPath();
    LRESULT buttonType;
    COLORREF hoverColor, pushedColor, hoverInactiveColor;
    CMPCThemeFrameUtil* parent;
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnBnClicked();
};

