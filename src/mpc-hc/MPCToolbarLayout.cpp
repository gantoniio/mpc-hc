#include "stdafx.h"
#include "MPCToolbarLayout.h"
#include "mplayerc.h"
#include "AppSettings.h"

static void GetStr(LPCSTR& pszBuff, int& nLength, CHAR sep, LPCSTR& pszMatch, int& nMatchLength) {
    // Trim left whitespace
    while (CString::StrTraits::IsSpace(*pszBuff)) {
        pszBuff++;
        nLength--;
    }

    LPCSTR pEnd = CStringA::StrTraits::StringFindChar(pszBuff, sep);
    if (pEnd == nullptr) {
        if (nLength < 1) {
            throw 1;
        }
        nMatchLength = nLength;
    } else {
        nMatchLength = int(pEnd - pszBuff);
    }

    pszMatch = pszBuff;
    if (nMatchLength < nLength) {
        pszBuff = pEnd + 1;
        nLength -= nMatchLength + 1;
    }
}

using regexResult = std::vector<std::string>;
using regexResults = std::vector<regexResult>;

size_t stringMatch(const std::regex& pattern, const std::string& text, regexResults& results) {
    results.clear();

    std::string data(text);
    std::smatch match_pieces;
    while (std::regex_search(data, match_pieces, pattern)) {
        regexResult result;
        for (const auto& match : match_pieces) {
            if (match != *match_pieces.begin()) {
                result.push_back(match.str());
            }
        }
        results.push_back(result);
        data = match_pieces.suffix().str();
    }
    return results.size();
}

MPCToolbarLayout& operator<<=(MPCToolbarLayout& layout, const CString& layoutStr) {
    const auto& s = AfxGetAppSettings();

    regexResults results;
    std::regex regex_pattern("<([A-Z0-9_]*)>");
    stringMatch(regex_pattern, (const char*)UTF16To8(layoutStr), results);

    for (const auto& iter : results) {
        CStringA buttonID = iter[0].c_str();
        if (s.CommandStrToID.count(buttonID)) { //found buttonid
            layout.buttons.push_back(s.CommandStrToID[buttonID]);
        }
    }
    return layout;
}

CString& operator<<=(CString& layoutStr, const MPCToolbarLayout& l) {
    const auto& s = AfxGetAppSettings();
    layoutStr = _T("");
    for (std::vector<int>::size_type i = 0; i < l.buttons.size(); ++i) {
        if (s.CommandIDToWMCMD.count(l.buttons[i])>0) {
            CStringA t;
            t.Format("<%s>", s.CommandIDToWMCMD[l.buttons[i]]->cmdidStr);
            layoutStr += CA2W(t, CP_UTF8);
        } else if (l.buttons[i]== ID_DUMMYSEPARATOR) {
            layoutStr += _T("<DUMMYSEPARATOR>");
        } else if (l.buttons[i] == ID_BUTTONSEP) {
            layoutStr += _T("<BUTTONSEP>");
        }
    }
    return layoutStr;
}
