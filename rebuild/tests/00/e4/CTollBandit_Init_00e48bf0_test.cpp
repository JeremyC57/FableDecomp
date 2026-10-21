// Behaviour test for CTollBandit::Init (0x00E48BF0): toll fields reset, pay-me distance from the script def, the escape
// marker looked up by name, then the faction and wanting-money calls on Me.
#include <cstdio>
#include <cstring>

struct CScriptThing { int Id; };
static char g_lookup[32], g_faction[32]; static int g_money = -1; static const CScriptThing* g_factionThing;
struct CGameScriptInterface {
    CScriptThing GetThingWithScriptName(const char* name) const { std::strcpy(g_lookup, name); CScriptThing t = { 42 }; return t; }
    void EntitySetInFaction(const CScriptThing& t, const char* f) const { g_factionThing = &t; std::strcpy(g_faction, f); }
    void EntitySetThingAsWantingMoney(const CScriptThing&, long amount) const { g_money = (int)amount; }
};
struct CScriptDef { long TollBanditPayMeDistance; };
static CScriptDef g_def = { 15 };
static CScriptDef* g_pScriptDef = &g_def;
struct CTollBandit {
    CGameScriptInterface* Game; CScriptThing Me; long TollState; bool TollBanditOnlyCollectsToll; bool KilledReluctantTrader;
    long TollBanditPayMeDistance; CScriptThing BanditEscapeMarker;
    void Init()
    {
        TollState = 1;
        TollBanditOnlyCollectsToll = false;
        KilledReluctantTrader = false;
        TollBanditPayMeDistance = g_pScriptDef->TollBanditPayMeDistance;
        BanditEscapeMarker = Game->GetThingWithScriptName("M_BanditEscapePoint");
        Game->EntitySetInFaction(Me, "FACTION_BANDITS_FRIENDLY");
        Game->EntitySetThingAsWantingMoney(Me, 0);
    }
};

int main()
{
    CGameScriptInterface gsi;
    CTollBandit b; b.Game = &gsi; b.Me.Id = 7; b.TollState = 0; b.TollBanditOnlyCollectsToll = true; b.KilledReluctantTrader = true;
    b.TollBanditPayMeDistance = 0; b.BanditEscapeMarker.Id = 0;
    b.Init();
    bool ok = b.TollState == 1 && !b.TollBanditOnlyCollectsToll && !b.KilledReluctantTrader && b.TollBanditPayMeDistance == 15;
    ok = ok && b.BanditEscapeMarker.Id == 42 && std::strcmp(g_lookup, "M_BanditEscapePoint") == 0;
    ok = ok && g_factionThing == &b.Me && std::strcmp(g_faction, "FACTION_BANDITS_FRIENDLY") == 0 && g_money == 0;
    if (!ok) { std::printf("00e48bf0_TEST FAIL\n"); return 1; }
    std::printf("00e48bf0_TEST PASS\n");
    return 0;
}