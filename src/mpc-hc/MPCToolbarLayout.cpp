#include "stdafx.h"
#include "MPCToolbarLayout.h"
#include "RegexUtil.h"
#include "AppSettings.h"


MPCToolbarLayout& operator <<= (MPCToolbarLayout& tbLayout, const CStringW& strLayout) {
    RegexUtil::wregexResults results;
    std::wregex tbPattern(LR"(<\|([^<>\|]*)\|([0-9-]*)\|>)");
    tbLayout.buttons.clear();
    tbLayout.SVGIndex.clear();
    RegexUtil::wstringMatch(tbPattern, (const wchar_t*)strLayout, results);
    if (results.size() > 0) {
        for (RegexUtil::wregexResult tbElement: results) {
            if (tbElement.size() == 2) {
                int index = -1;
                if (StrToInt32(tbElement[1].c_str(), index)) {
                    int id = -1;
                    if (tbElement[0] == L"BUTTONSEP") {
                        id = ID_BUTTONSEP;
                    } else if (tbElement[0] == L"DUMMYSEPARATOR") {
                        id = ID_DUMMYSEPARATOR;
                    } else if (CAppSettings::CommandStrToID.count(tbElement[0].c_str()) > 0) {
                        id = CAppSettings::CommandStrToID[tbElement[0].c_str()];
                    }
                    if (-1 != id) {
                        tbLayout.buttons.push_back(id);
                        tbLayout.SVGIndex.push_back(index);
                    }
                }
            }
        }
    }

    return tbLayout;
}

CStringW& operator <<= (CStringW& strLayout, const MPCToolbarLayout& tbLayout) {
    if (tbLayout.buttons.size() > 0 && tbLayout.SVGIndex.size() == tbLayout.buttons.size()) {
        strLayout = L"";
        for (std::vector<int>::size_type i = 0; i < tbLayout.buttons.size(); ++i) {
            int button = tbLayout.buttons[i];
            if (CAppSettings::CommandIDToWMCMD.count(button) || button==ID_BUTTONSEP || button==ID_DUMMYSEPARATOR) {
                CStringW t;
                int idx = tbLayout.SVGIndex[i];
                if (button == ID_BUTTONSEP) {
                    t.Format(L"<|BUTTONSEP|%d|>", idx);
                } else if (button == ID_DUMMYSEPARATOR) {
                    t.Format(L"<|DUMMYSEPARATOR|%d|>", idx);
                } else {
                    const char* cmdStr = CAppSettings::CommandIDToWMCMD[button]->cmdidStr;
                    t.Format(L"<|%hs|%d|>", cmdStr, idx);
                }
                strLayout += t;
            }
        }
    } else {
        strLayout = MPCToolbarLayout::defaultLayout;
    }

    return strLayout;
}
