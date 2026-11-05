// Behaviour test for 0x00899EF0 CGameScriptInterface::GiveHeroMorality (plain C++, links the row object; the row's TU
// carries the CVectorMap LowerBound body its forced-inline GetTC calls).
#include <stdio.h>
#include <string.h>
struct SEntry { long Type; void* Component; };
class CThingPlayerCreature { public:
    unsigned char Pad00[0x20];
    unsigned long UsingComponents[9];                 // +0x20
    SEntry* Start; SEntry* Finish; SEntry* EndOfStorage; unsigned char Compare; unsigned char Dirty; unsigned char Pad52[2];  // +0x44
    unsigned char Pad54[0x91 - 0x54];
    unsigned char Flags91;                            // +0x91 bit 0 Dead
};
class CTCHeroStats { public:
    unsigned char Pad[0x28]; long Morality; long Max; long SetCalls;
    long GetMoralityMax() const;
    void SetMorality(long morality);
};
long CTCHeroStats::GetMoralityMax() const { return Max; }
void CTCHeroStats::SetMorality(long morality) { Morality = morality; ++SetCalls; }
static CThingPlayerCreature* g_hero = 0;
static int g_mainCalls = 0;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return g_hero; }
static CPlayer g_player;
class CPlayerManager { public: CPlayer& GetMainPlayer(); };
CPlayer& CPlayerManager::GetMainPlayer() { ++g_mainCalls; return g_player; }
class CGameScriptInterface { public:
    virtual void GiveHeroMorality(float amount) const;
    unsigned char Pad04[0x10];
    CPlayerManager* PlayerManager;                    // +0x14
};
static bool Check(CGameScriptInterface& gsi, CTCHeroStats& stats, long start, long max, float amount, long expect)
{
    stats.Morality = start; stats.Max = max; stats.SetCalls = 0;
    gsi.CGameScriptInterface::GiveHeroMorality(amount);
    return stats.Morality == expect && stats.SetCalls == 1;
}
int main()
{
    CPlayerManager manager;
    CGameScriptInterface gsi; memset(gsi.Pad04, 0, sizeof gsi.Pad04); gsi.PlayerManager = &manager;
    CTCHeroStats stats; memset(&stats, 0, sizeof stats);
    CThingPlayerCreature hero; memset(&hero, 0, sizeof hero);
    SEntry entries[2] = { { 2, 0 }, { 4, &stats } };
    hero.Start = entries; hero.Finish = entries + 2; hero.EndOfStorage = entries + 2;
    hero.UsingComponents[0] = 1UL << 4;
    g_hero = &hero;
    if (!Check(gsi, stats, 10, 7, 0.5f, 14)) { printf("FAIL round half up %ld\n", stats.Morality); return 1; }     // 3.5 -> 4
    if (!Check(gsi, stats, 10, 7, -0.5f, 6)) { printf("FAIL negative %ld\n", stats.Morality); return 1; }       // -3.5 -> -4
    if (!Check(gsi, stats, 0, 5, 0.5f, 2)) { printf("FAIL ties even %ld\n", stats.Morality); return 1; }        // 2.5 -> 2
    if (!Check(gsi, stats, 100, 1000, 0.1f, 200)) { printf("FAIL scale %ld\n", stats.Morality); return 1; }
    hero.Flags91 = 1;                                  // dead hero: nothing happens
    stats.Morality = 33; stats.SetCalls = 0; gsi.CGameScriptInterface::GiveHeroMorality(1.0f);
    if (stats.Morality != 33 || stats.SetCalls != 0) { printf("FAIL dead\n"); return 1; }
    g_hero = 0; g_mainCalls = 0;                       // no hero
    gsi.CGameScriptInterface::GiveHeroMorality(1.0f);
    if (stats.SetCalls != 0 || g_mainCalls != 1) { printf("FAIL no hero\n"); return 1; }
    printf("CGameScriptInterface_00899ef0_TEST PASS\n");
    return 0;
}
