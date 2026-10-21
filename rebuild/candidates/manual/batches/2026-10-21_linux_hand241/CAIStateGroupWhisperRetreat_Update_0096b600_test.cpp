#include <cstdio>
#include <math.h>
// Behaviour copy of CAIStateGroup_WhisperRetreat::Update (0x0096B600).
struct Thing { bool dead; bool hasStealth; float x, y; int dangerCalls; long dangerStatus; };
struct Boss { long phase; bool ready; long nextPhase; };
struct Group {
    Thing* creature; Thing* target; Boss* boss; bool armed;
    int unsheathes, jumps, wills, terminates;
    void Update() {
        if (!armed) ++unsheathes;
        if (target != 0 && !target->dead) {
            if (target->hasStealth) { ++target->dangerCalls; target->dangerStatus = 3; }
            float dx = target->x - creature->x, dy = target->y - creature->y;
            if ((float)sqrt(dx * dx + dy * dy) < 3.0f) ++jumps;
            switch (boss->phase) {
            case 0: if (boss->ready) boss->nextPhase = 1; break;
            case 2: ++wills; ++terminates; return;
            default: break;
            }
        }
        ++terminates;
    }
};
int main()
{
    Thing c = { false, false, 0, 0, 0, 0 }; Thing t = { false, true, 1, 2, 0, 0 }; Boss b = { 0, true, -1 };
    Group g = { &c, &t, &b, false, 0, 0, 0, 0 };
    g.Update();
    if (g.unsheathes != 1 || t.dangerCalls != 1 || t.dangerStatus != 3 || g.jumps != 1 || b.nextPhase != 1 || g.terminates != 1 || g.wills != 0) { std::printf("0096b600_TEST FAIL\n"); return 1; }
    g.armed = true; t.x = 10; b.phase = 2; b.nextPhase = -1; t.hasStealth = false;
    g.Update();
    if (g.unsheathes != 1 || t.dangerCalls != 1 || g.jumps != 1 || g.wills != 1 || g.terminates != 2 || b.nextPhase != -1) { std::printf("0096b600_TEST FAIL\n"); return 1; }
    t.dead = true; b.phase = 2; g.Update();
    if (g.wills != 1 || g.terminates != 3) { std::printf("0096b600_TEST FAIL\n"); return 1; }
    g.target = 0; b.phase = 0; b.ready = false; g.Update();
    if (g.terminates != 4 || b.nextPhase != -1) { std::printf("0096b600_TEST FAIL\n"); return 1; }
    std::printf("0096b600_TEST PASS\n"); return 0;
}
