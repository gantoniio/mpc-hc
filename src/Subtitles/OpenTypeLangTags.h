#pragma once

constexpr int OTLangTagLen = 4; //see here: https://docs.microsoft.com/en-us/typography/opentype/spec/languagetags
constexpr int OTLangHintLen = 3; //see harfbuzz hb-ot-tag-table.hh

class OpenTypeLang {
public:
    typedef char TagStr[OTLangTagLen + 1];
    typedef char HintStr[OTLangHintLen + 1];
    typedef struct OpenTypeLangTag {
        wchar_t* langDescription;
        TagStr otTag;
        HintStr lang;
    } T;
    static OpenTypeLangTag OpenTypeLangTags[603];
};
