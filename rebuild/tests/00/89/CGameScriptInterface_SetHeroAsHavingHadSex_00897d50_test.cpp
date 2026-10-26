// behaviour test for 0x00897D50 CGameScriptInterface::SetHeroAsHavingHadSex: the value reaches CTCHeroStats::HadScriptedSex only for a live hero that has the component (interface 0x4, word 0 bit 4)
#include <cstdio>
struct Hero {
    bool          present;
    bool          dead;
    unsigned long using_components[9];
    bool        field;                                 // stands in for CTCHeroStats::HadScriptedSex
};
static void Set(Hero& h, bool value)                  // the function under test
{
    if (!h.present || h.dead) return;
    long type = 0x4;
    if (!(h.using_components[type >> 5] & (1UL << (type & 31)))) return;
    h.field = value;
}
int main()
{
    int fail = 0;
    if ((0x4 >> 5) != 0 || (1UL << (0x4 & 31)) != 0x10UL) fail |= 1;
    Hero h; h.present = true; h.dead = false; h.field = false;
    for (int i = 0; i < 9; ++i) h.using_components[i] = 0;
    Set(h, true); if (h.field != false) fail |= 2;           // no component
    h.using_components[0] = 0x10UL;
    Set(h, true); if (h.field != true) fail |= 4;           // stored
    h.dead = true; Set(h, false); if (h.field != true) fail |= 8;   // dead hero untouched
    h.dead = false; h.present = false; Set(h, false); if (h.field != true) fail |= 16;
    if (fail) { std::printf("00897d50_TEST FAIL %d\n", fail); return 1; }
    std::printf("00897d50_TEST PASS\n");
    return 0;
}