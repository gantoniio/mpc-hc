#pragma once
#include "stdafx.h"
#include "CMPCTheme.h"
class CMPCThemeFrameUtil {
public:
    CMPCThemeFrameUtil();
    COLORREF titleBarColor;
    void setTitleBarColor(COLORREF taskbarColor);
    COLORREF getTitleBarColor() { return titleBarColor; };
};

