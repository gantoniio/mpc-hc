#pragma once

class MPCToolbarLayout {
public:
    std::vector<int> buttons;
    std::vector<int> SVGIndex;
    static constexpr wchar_t *defaultLayout = L"<|PLAY_PLAY|-1|><|PLAY_PAUSE|-1|><|PLAY_STOP|-1|><|BUTTONSEP|-1|><|NAVIGATE_SKIPBACK|-1|><|PLAY_DECRATE|-1|><|PLAY_INCRATE|-1|><|NAVIGATE_SKIPFORWARD|-1|><|BUTTONSEP|-1|><|PLAY_FRAMESTEP|-1|><|BUTTONSEP|-1|><|DUMMYSEPARATOR|-1|><|VOLUME_MUTE|-1|>";
    friend CStringW& operator <<= (CStringW& strLayout, const MPCToolbarLayout& tbLayout);
    friend MPCToolbarLayout& operator <<= (MPCToolbarLayout& tbLayout, const CStringW& strLayout);
};
