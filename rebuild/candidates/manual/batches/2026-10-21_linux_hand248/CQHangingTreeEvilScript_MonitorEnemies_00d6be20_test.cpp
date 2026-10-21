// Behaviour test for 00d6be20 (CQ_HangingTreeEvilScript::MonitorEnemies): when all enemies die the flag is set; when
// the hero switches regions first, the monitor waits for the switch, re-collects HTE_AssaultGuard enemies, sets the
// flag (the list is fresh) and respawns itself only if some of them still live.
#include <cstdio>
#include <cstring>

struct Game { int frames, termAt, deadAt, switchFrom, switchTo, respawns, collects; int aliveAfterCollect; };
struct Script {
    Game* g; bool Switching, AllDead; int alive;
    bool Term() { return g->frames >= g->termAt; }
    bool AreAllDead() { return alive == 0; }
    void Frame()
    {
        ++g->frames;
        if (g->frames == g->deadAt) alive = 0;
        Switching = g->frames >= g->switchFrom && g->frames < g->switchTo;
    }
    void MonitorEnemies()
    {
        AllDead = false;
        while (!AreAllDead()) {
            Frame();
            if (Term()) return;
            if (Switching) {
                if (Term()) return;
                while (Switching) { Frame(); if (Term()) return; }
                if (Term()) return;
                alive = 0; AllDead = true;
                ++g->collects; alive = g->aliveAfterCollect;
                if (AreAllDead()) return;
                if (Term()) return;
                ++g->respawns;
                return;
            }
        }
        if (Term()) return;
        AllDead = true;
    }
};
int main()
{
    Game g; std::memset(&g, 0, sizeof g); g.termAt = 100; g.deadAt = 5; g.switchFrom = 1000;
    Script s; s.g = &g; s.Switching = false; s.alive = 3; s.MonitorEnemies();
    int ok = s.AllDead && g.respawns == 0 && g.collects == 0;
    Game h; std::memset(&h, 0, sizeof h); h.termAt = 100; h.deadAt = 1000; h.switchFrom = 3; h.switchTo = 6; h.aliveAfterCollect = 2;
    Script t; t.g = &h; t.Switching = false; t.alive = 3; t.MonitorEnemies();
    if (h.collects != 1 || h.respawns != 1 || !t.AllDead) ok = 0;
    if (!ok) { std::printf("00d6be20_TEST FAIL\n"); return 1; }
    std::printf("00d6be20_TEST PASS\n");
    return 0;
}
