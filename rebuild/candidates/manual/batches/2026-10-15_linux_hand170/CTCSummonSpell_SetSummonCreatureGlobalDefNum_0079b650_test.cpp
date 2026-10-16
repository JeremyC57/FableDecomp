#include <cstdio>
// Behaviour model of CTCSummonSpell::SetSummonCreatureGlobalDefNum (0x0079B650): the global def number is recorded, the upgrade
// sound plays when the caster has a sound player, the old minion is killed and a new one created.
static long g_defNum = 0;
struct Log { int sounds, kills, creates; };
static void Set(Log& l, long defNum, bool hasSound)
{
    g_defNum = defNum;
    if (hasSound) l.sounds++;
    l.kills++;
    l.creates++;
}
int main()
{
    Log l = { 0, 0, 0 };
    Set(l, 42, true); Set(l, 7, false);
    if (g_defNum != 7 || l.sounds != 1 || l.kills != 2 || l.creates != 2) { std::printf("0079b650_TEST FAIL\n"); return 1; }
    std::printf("0079b650_TEST PASS\n");
    return 0;
}
