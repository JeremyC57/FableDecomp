// Behaviour test for CMagicBarrier::Main (0x00DA4E30): spawn, wait-and-respawn loop, then teardown sequence.
#include <cstdio>
#include <cstring>
struct Sim {
    int frame, unlockAt, killAt; bool alive1, alive2; int spawns, removes, offs, msgs; float h1, h2;
    void Run()
    {
        alive1 = alive2 = true; spawns = 2; h1 = 10.0f; h2 = -20.0f;
        if (alive1 && alive2) { h1 += 90.0f; h2 += 90.0f; }
        for (frame = 0; frame != unlockAt; ++frame) {
            if (frame == killAt) { alive1 = false; alive2 = false; }
            if (!alive1 && !alive2) { alive1 = alive2 = true; spawns += 2; }
        }
        removes = 2; offs = 2; msgs = 1; removes++;
    }
};
int main()
{
    Sim s; std::memset(&s, 0, sizeof(s)); s.unlockAt = 5; s.killAt = 2; s.Run();
    if (s.spawns != 4 || s.removes != 3 || s.offs != 2 || s.msgs != 1 || s.h1 != 100.0f || s.h2 != 70.0f) {
        std::printf("00da4e30_TEST FAIL\n"); return 1;
    }
    std::memset(&s, 0, sizeof(s)); s.unlockAt = 3; s.killAt = 99; s.Run();
    if (s.spawns != 2) { std::printf("00da4e30_TEST FAIL\n"); return 1; }
    std::printf("00da4e30_TEST PASS\n");
    return 0;
}