// Behaviour test for 0x00dfc290 (CQ_TraderConflictGoodScript::WatchForKilledPeople): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Data { bool flag126; };
struct Script { long Reached; Data* data; int frame; int killFrame; int terminateFrame; int updates;
    bool IsActiveThreadTerminating() { return frame >= terminateFrame; }
    void Update() { ++frame; ++updates; if (frame == 4) Reached = 3; }
    bool HeroKilledSomeone() { return frame == killFrame; }
    void Watch() { while (Reached < 3) { Update(); if (IsActiveThreadTerminating()) return; if (HeroKilledSomeone()) { if (IsActiveThreadTerminating()) return; data->flag126 = false; } }
        if (IsActiveThreadTerminating()) return; } };

int main()
{
    Data d; d.flag126 = true; Script s; s.Reached = 0; s.data = &d; s.frame = 0; s.killFrame = 2; s.terminateFrame = 100; s.updates = 0;
    s.Watch(); CHECK(!d.flag126 && s.updates == 4);
    d.flag126 = true; s.Reached = 0; s.frame = 0; s.terminateFrame = 1; s.updates = 0; s.Watch(); CHECK(d.flag126 && s.updates == 1);
    s.Reached = 3; s.updates = 0; s.Watch(); CHECK(s.updates == 0);
    if (g_fail) { std::printf("00dfc290_TEST FAIL\n"); return 1; }
    std::printf("00dfc290_TEST PASS\n");
    return 0;
}
