// Behaviour test for 00ec5140 (CV_RockTrollFirstEncounterScript::WatchForRockTrollHit): yield until the hero hits the
// troll (melee or special ability, null implementation counts as "not hit"), then show the health bar only while the
// troll still has health; a terminating thread returns early.
#include <cstdio>
#include <cstring>

struct Impl { int hitAt, abilityAt; int* frames; };
struct Thing {
    Impl* data;
    bool MsgIsHitBy(const char* who) const { if (!data) return false; return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && *data->frames >= data->hitAt; }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const char* who) const { if (!data) return false; return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && *data->frames >= data->abilityAt; }
};
struct Game {
    int frames; float health; int bars; bool shown;
    void NewScriptFrame() { ++frames; }
    float GetHealth(const Thing&) const { return health; }
    long AddQuestInfoBarHealth(const Thing&, int r, int g, int b, int a) { ++bars; return r == 255 && g == 0 && b == 0 && a == 255 ? 77 : -1; }
    void DisplayQuestInfo(bool s) { shown = s; }
};
struct Script {
    Game* game; long barId; int termAt;
    bool IsActiveThreadTerminating() { return game->frames >= termAt; }
    void WatchForRockTrollHit(Thing troll)
    {
        while (!(troll.MsgIsHitBy("SCRIPT_NAME_HERO") || troll.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO"))) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (game->GetHealth(troll) > 0.0001f) {
            if (IsActiveThreadTerminating())
                return;
            barId = game->AddQuestInfoBarHealth(troll, 255, 0, 0, 255);
            game->DisplayQuestInfo(true);
        }
    }
};
int main()
{
    int ok = 1;
    Game g = { 0, 50.0f, 0, false };
    Impl im = { 1000, 3, &g.frames };
    Thing t = { &im };
    Script s = { &g, 0, 100 };
    s.WatchForRockTrollHit(t);
    if (g.frames != 3 || s.barId != 77 || g.bars != 1 || !g.shown) ok = 0;
    Game h = { 0, 0.0f, 0, false };
    Impl im2 = { 2, 1000, &h.frames };
    Thing u = { &im2 };
    Script v = { &h, 5, 100 };
    v.WatchForRockTrollHit(u);
    if (h.frames != 2 || v.barId != 5 || h.bars != 0 || h.shown) ok = 0;
    Game k = { 0, 50.0f, 0, false };
    Thing w = { 0 };
    Script x = { &k, 5, 4 };
    x.WatchForRockTrollHit(w);
    if (k.frames != 4 || k.bars != 0) ok = 0;
    if (!ok) { std::printf("00ec5140_TEST FAIL\n"); return 1; }
    std::printf("00ec5140_TEST PASS\n");
    return 0;
}