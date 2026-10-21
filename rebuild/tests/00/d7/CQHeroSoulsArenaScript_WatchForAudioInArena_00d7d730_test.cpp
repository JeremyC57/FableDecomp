// Behaviour test for 00d7d730 (CQ_HeroSoulsArenaScript::WatchForAudioInArena): the arena music override starts once
// when either arena region is loaded and stops once when neither is; a frame passes every iteration until terminating.
#include <cstdio>
#include <cstring>

struct Game { int frames, termAt; int overrides, stops; long lastSet; bool cells[32], hall[32]; };
struct Script {
    Game* g;
    bool Term() { return g->frames >= g->termAt; }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "ArenaCells") == 0 ? g->cells[g->frames] : g->hall[g->frames]; }
    void Run()
    {
        bool on = false;
        while (!Term()) {
            if (!on && (IsRegionLoaded("ArenaCells") || IsRegionLoaded("ArenaHallOfHeroes"))) {
                if (Term()) return;
                on = true; ++g->overrides; g->lastSet = 0x22;
            } else if (on && !(IsRegionLoaded("ArenaCells") || IsRegionLoaded("ArenaHallOfHeroes"))) {
                if (Term()) return;
                on = false; ++g->stops;
            }
            ++g->frames;
        }
    }
};
int main()
{
    Game g; std::memset(&g, 0, sizeof g); g.termAt = 20;
    for (int i = 2; i < 6; ++i) g.cells[i] = true;
    for (int i = 5; i < 9; ++i) g.hall[i] = true;
    for (int i = 12; i < 14; ++i) g.hall[i] = true;
    Script s; s.g = &g; s.Run();
    int ok = g.overrides == 2 && g.stops == 2 && g.lastSet == 0x22 && g.frames == 20;
    if (!ok) { std::printf("00d7d730_TEST FAIL\n"); return 1; }
    std::printf("00d7d730_TEST PASS\n");
    return 0;
}