// Behaviour test for 0x0049A0B0 CProgressDisplay::InitialiseTextures: nothing happens during an autosave display, without a display
// manager or when the textures exist. The debug display loads only the splash; otherwise full screen loads two shuffled backgrounds
// from Fable_BG_PC_02..09 (one when reduced) and, unless reduced, 16 guild seals guild_seal_01..16 (one with a single background),
// then the four slot / bar textures and the two borders. The text tags are read and shuffled in full screen, and the first tip text
// is chosen when none is set: a tag ending in "0" drops its last three characters for the title symbol.
#include <cstdio>
#include <string>
#include <vector>

struct Display {
    std::vector<std::string> loaded; std::string tip, title; bool have_manager, autosave, debug, full, have_textures;
    Display() : have_manager(false), autosave(false), debug(false), full(false), have_textures(false) { }
    static std::string Num(long n) { char b[16]; std::sprintf(b, "%ld", n); return b; }
    void Init(bool single_background, bool reduced, const std::vector<long>& shuffled, const std::string& tag)
    {
        if (autosave || !have_manager || have_textures) return;
        if (debug) { loaded.push_back("splash.tga"); return; }
        if (full) {
            long count = reduced ? 1 : 2;
            for (long i = 0; i < count; ++i) loaded.push_back("Fable_BG_PC_0" + Num(shuffled[i]) + ".dds");
        }
        if (full && !reduced) {
            long seals = single_background ? 1 : 16;
            for (long i = 0; i < seals; ++i) loaded.push_back(std::string("guild_seal_") + (i + 1 < 10 ? "0" : "") + Num(i + 1) + ".dds");
            loaded.push_back("slot_left_bar_off.dds"); loaded.push_back("slot_mid_bar_off.dds");
            loaded.push_back("slot_right_bar_off.dds"); loaded.push_back("Ybar_seg.dds");
            loaded.push_back("fable_BG_bar_border.dds"); loaded.push_back("rc_seal_border.dds");
        }
        if (full && tip.empty()) {
            std::string t = tag;
            if (t.size() >= 1 && t[t.size() - 1] == '0') t = t.substr(0, t.size() - 3);
            tip = tag; title = t + "_TITLE";
        }
    }
};

int main()
{
    long s[] = { 5, 2, 9, 3, 4, 6, 7, 8 };
    std::vector<long> shuffled(s, s + 8);
    Display a; a.have_manager = true; a.full = true;
    a.Init(false, false, shuffled, "TEXT_TIP_010");
    bool ok = a.loaded.size() == 2 + 16 + 6 && a.loaded[0] == "Fable_BG_PC_05.dds" && a.loaded[2] == "guild_seal_01.dds"
        && a.loaded[17] == "guild_seal_16.dds" && a.loaded[23] == "rc_seal_border.dds" && a.title == "TEXT_TIP__TITLE" && a.tip == "TEXT_TIP_010";
    Display b; b.have_manager = true; b.full = true;
    b.Init(true, true, shuffled, "TIP_5");
    ok = ok && b.loaded.size() == 1 && b.title == "TIP_5_TITLE";
    Display c; c.have_manager = true; c.debug = true;
    c.Init(false, false, shuffled, "X");
    ok = ok && c.loaded.size() == 1 && c.loaded[0] == "splash.tga";
    Display d; d.have_manager = true; d.autosave = true;
    d.Init(false, false, shuffled, "X");
    ok = ok && d.loaded.empty();
    std::printf(ok ? "0049a0b0_TEST PASS\n" : "0049a0b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
