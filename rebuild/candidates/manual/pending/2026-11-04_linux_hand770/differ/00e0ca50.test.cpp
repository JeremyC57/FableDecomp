// Behaviour test for NScript::CQ_UndeadRisingScript::CUndeadRisingFirefly::Main 0x00E0CA50 (parked): a model of the script loop --
// wait for quest state 1, raise a zombie at a grave, wait for it to die, then every 7 timer ticks raise another at a fresh grave
// while the quest stays in state 1; termination ends the thread.
#include <cstdio>
struct World { int state; int frame; int zombiesRaised; int zombieHp; int timer; int terminateAt; };
static bool Frame(World& w) { ++w.frame; if (w.timer > 0) --w.timer; if (w.zombieHp > 0) --w.zombieHp; if (w.frame == 3) w.state = 1; if (w.frame == 40) w.state = 2; return w.frame >= w.terminateAt; }
static void Main(World& w)
{
    while (w.state != 1) { if (Frame(w)) return; }
    w.timer = 0;
    if (w.state == 1) {
        ++w.zombiesRaised; w.zombieHp = 4;
        while (w.zombieHp > 0) { if (Frame(w)) return; }
        w.timer = 7;
    }
    while (w.state == 1) {
        if (Frame(w)) return;
        while (w.timer > 0) { if (Frame(w)) return; }
        if (w.state != 1) break;
        ++w.zombiesRaised; w.zombieHp = 4;
        while (w.zombieHp > 0) { if (Frame(w)) return; }
        w.timer = 7;
    }
}
int main()
{
    World w = { 0, 0, 0, 0, 0, 1000 };
    Main(w);
    if (w.zombiesRaised < 3 || w.state != 2) { std::printf("00e0ca50_TEST FAIL\n"); return 1; }
    World t = { 0, 0, 0, 0, 0, 2 };
    Main(t);
    if (t.zombiesRaised != 0) { std::printf("00e0ca50_TEST FAIL\n"); return 1; }
    std::printf("00e0ca50_TEST PASS\n");
    return 0;
}
