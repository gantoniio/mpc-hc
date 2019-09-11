#include "stdafx.h"
#include "CMPCThemeFrameUtil.h"

CMPCThemeFrameUtil::CMPCThemeFrameUtil():titleBarColor(RGB(0,0,0)) {
}

void CMPCThemeFrameUtil::setTitleBarColor(COLORREF _titleBarColor) {
    this->titleBarColor = _titleBarColor;
}
