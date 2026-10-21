// Behaviour test for 0x00E50E40 (CV_BeardyBaldyScript::GoTalkToBeardyBaldy): the mirror marker swap (off Beardy, onto the barber).
#include <cstdio>
#include <cstring>
// Host model: things carry an alive flag and a tag; the game interface logs marker calls as
// "+<tag>:<marker>" / "-<tag>"; terminating is checked through a countdown.
struct Thing { char tag; bool alive; bool IsAlive() const { return alive; } };
struct Game {
    char log[256]; Thing beardy; Thing barber; Thing hero; const Thing* nearArg; char nearDef[96];
    Game() : nearArg(0) { log[0] = 0; nearDef[0] = 0; hero.tag = 'H'; hero.alive = true; }
    void Add(const char* s) { std::strcat(log, s); }
    const Thing& GetHero() { return hero; }
    Thing GetThingWithScriptName(const char* name) const
    {
        if (std::strcmp(name, "BB_BeardyBaldyMan") == 0) return beardy;
        Thing none; none.tag = '?'; none.alive = false; return none;
    }
    Thing GetNearestWithDefName(const Thing& t, const char* def)
    {
        nearArg = &t; std::strcpy(nearDef, def); return barber;
    }
    void MiniMapAddMarker(const Thing& t, const char* marker)
    {
        char b[64]; std::sprintf(b, "+%c:%s;", t.tag, marker); Add(b);
    }
    void MiniMapRemoveMarker(const Thing& t) { char b[8]; std::sprintf(b, "-%c;", t.tag); Add(b); }
};
struct Script {
    Game* GSI; int termAfter; int checks;
    Script(Game* g, int after) : GSI(g), termAfter(after), checks(0) {}
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void GoTalkToBeardyBaldy()
    {
        Thing beardy = GSI->GetThingWithScriptName("BB_BeardyBaldyMan");
        Thing barber = GSI->GetNearestWithDefName(GSI->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
        if (beardy.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            GSI->MiniMapRemoveMarker(beardy);
        }
        if (barber.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            GSI->MiniMapAddMarker(barber, "HUD_ORB_QUEST_VIGNETTE");
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static const char* Run(bool beardyAlive, bool barberAlive, int termAfter, Game& g)
{
    g.beardy.tag = 'B'; g.beardy.alive = beardyAlive; g.barber.tag = 'L'; g.barber.alive = barberAlive;
    Script s(&g, termAfter); s.GoTalkToBeardyBaldy(); return g.log;
}
int main()
{
    { Game g; CHECK(std::strcmp(Run(true, true, 9, g), "-B;+L:HUD_ORB_QUEST_VIGNETTE;") == 0);
      CHECK(g.nearArg == &g.hero && std::strcmp(g.nearDef, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER") == 0); }
    { Game g; CHECK(std::strcmp(Run(false, true, 9, g), "+L:HUD_ORB_QUEST_VIGNETTE;") == 0); }
    { Game g; CHECK(std::strcmp(Run(true, false, 9, g), "-B;") == 0); }
    { Game g; CHECK(std::strcmp(Run(false, false, 0, g), "") == 0); }
    { Game g; CHECK(std::strcmp(Run(true, true, 0, g), "") == 0); }      // terminating before the remove
    { Game g; CHECK(std::strcmp(Run(true, true, 1, g), "-B;") == 0); }  // before the add
    { Game g; CHECK(std::strcmp(Run(false, true, 0, g), "") == 0); }
    if (g_fail) { std::printf("00e50e40_TEST FAIL\n"); return 1; }
    std::printf("00e50e40_TEST PASS\n");
    return 0;
}
