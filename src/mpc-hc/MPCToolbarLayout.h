#pragma once

#include <regex>
#include <vector>

class MPCToolbarLayout {
public:
    std::vector<int> buttons;
    friend MPCToolbarLayout& operator <<= (MPCToolbarLayout& s, const CString& layoutStr);
    friend CString& operator <<= (CString& layoutStr, const MPCToolbarLayout& s);
};
