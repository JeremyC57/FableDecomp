// Behaviour test for CTCRegionDisplay::SetCurrentAreaName(const CCharString&, ...) (0x00639BA0): the tag resolves to a
// text entry, to the first entry of a group, or to itself; an info display of the name already showing changes
// nothing; otherwise the name is set, the time defaults from -1 to 5 and show-now forces full alpha.
#include <cstdio>
#include <string>
#include <map>
#include <vector>

struct CBank {
    std::map<std::string, int> ids;          // tag -> id (0 = unknown)
    std::map<int, int> types;                // id -> 0 text, else group
    std::map<int, std::string> texts;
    std::map<int, std::vector<int> > groups;
};

class CRegion { public:
    std::string Current, Extra;
    long FrameChanged, CurrentAlpha, LastAlpha;
    float DisplayTime;
    bool TimeOut, Info, Paused;
    CRegion() : Current("old"), Extra("x"), FrameChanged(0), CurrentAlpha(7), LastAlpha(7), DisplayTime(0), TimeOut(false),
                Info(false), Paused(false) { }
    void Set(CBank& bank, const std::string& tag, float time, bool info, bool showNow, bool paused, long frame)
    {
        if (tag != "") {
            std::string text;
            int id = bank.ids.count(tag) ? bank.ids[tag] : 0;
            if (id > 0) {
                if (bank.types[id] == 0)
                    text = bank.texts[id];
                else
                    text = bank.texts[bank.groups[id].front()];
            } else
                text = tag;
            if (info && text == Current)
                return;
            Current = text;
            Extra = "";
            FrameChanged = frame;
            LastAlpha = 0;
        } else {
            Current = "";
            Extra = "";
            CurrentAlpha = 0;
        }
        if (time == -1.0f)
            time = 5.0f;
        if (showNow)
            CurrentAlpha = LastAlpha = 255;
        DisplayTime = time;
        TimeOut = time != 0.0f;
        Info = info;
        Paused = paused;
    }
};

int main()
{
    CBank bank;
    bank.ids["T"] = 1; bank.types[1] = 0; bank.texts[1] = "Bowerstone";
    bank.ids["G"] = 2; bank.types[2] = 1; bank.groups[2].push_back(3); bank.texts[3] = "Oakvale";
    bool ok = true;
    CRegion r;
    r.Set(bank, "T", -1.0f, false, true, true, 10);
    ok = ok && r.Current == "Bowerstone" && r.DisplayTime == 5.0f && r.CurrentAlpha == 255 && r.LastAlpha == 255 && r.TimeOut;
    r.Set(bank, "G", 0.0f, false, false, false, 11);
    ok = ok && r.Current == "Oakvale" && r.LastAlpha == 0 && !r.TimeOut && r.FrameChanged == 11;
    r.Set(bank, "G", 3.0f, true, true, false, 12);           // already showing: untouched
    ok = ok && r.FrameChanged == 11 && r.DisplayTime == 0.0f;
    r.Set(bank, "Raw", 2.0f, true, false, false, 13);
    ok = ok && r.Current == "Raw" && r.Info && r.DisplayTime == 2.0f;
    r.Set(bank, "", 1.0f, false, false, false, 14);
    ok = ok && r.Current == "" && r.CurrentAlpha == 0;
    std::printf(ok ? "00639ba0_TEST PASS\n" : "00639ba0_TEST FAIL\n");
    return ok ? 0 : 1;
}