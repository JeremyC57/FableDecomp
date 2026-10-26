// behaviour test for 0x008984E0 CGameScriptInterface::SetWeaponOutCrimeEnabled: the value reaches CTCHeroOpinionDeedLog::WeaponOutCrimeEnabled only for a live hero that has the component (interface 0x51, word 2 bit 17)
#include <cstdio>
struct Hero {
    bool          present;
    bool          dead;
    unsigned long using_components[9];
    bool        field;                                 // stands in for CTCHeroOpinionDeedLog::WeaponOutCrimeEnabled
};
static void Set(Hero& h, bool value)                  // the function under test
{
    if (!h.present || h.dead) return;
    long type = 0x51;
    if (!(h.using_components[type >> 5] & (1UL << (type & 31)))) return;
    h.field = value;
}
int main()
{
    int fail = 0;
    if ((0x51 >> 5) != 2 || (1UL << (0x51 & 31)) != 0x20000UL) fail |= 1;
    Hero h; h.present = true; h.dead = false; h.field = false;
    for (int i = 0; i < 9; ++i) h.using_components[i] = 0;
    Set(h, true); if (h.field != false) fail |= 2;           // no component
    h.using_components[2] = 0x20000UL;
    Set(h, true); if (h.field != true) fail |= 4;           // stored
    h.dead = true; Set(h, false); if (h.field != true) fail |= 8;   // dead hero untouched
    h.dead = false; h.present = false; Set(h, false); if (h.field != true) fail |= 16;
    if (fail) { std::printf("008984e0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008984e0_TEST PASS\n");
    return 0;
}