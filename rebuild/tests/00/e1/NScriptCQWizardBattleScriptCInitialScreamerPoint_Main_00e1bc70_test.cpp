#include <cstdio>
// Behaviour model of CInitialScreamerPoint::Main (0x00E1BC70): each visit of the hero (within 20, then beyond 20 again)
// spawns one screamer while fewer than three are alive and the maze stands; the alive count rises for its lifetime.
struct Battle { bool mazeDefeated; long alive; int spawned; };
static void Visit(Battle& b, int lifetime, long& peak)
{
    if (b.mazeDefeated) return;
    if (b.alive < 3 && !b.mazeDefeated) {
        ++b.spawned;
        ++b.alive;
        if (b.alive > peak) peak = b.alive;
        for (int f = 0; f < lifetime; ++f) { }   // the screamer lives `lifetime` frames
        --b.alive;
    }
}
int main()
{
    Battle b; b.mazeDefeated = false; b.alive = 0; b.spawned = 0;
    long peak = 0;
    Visit(b, 5, peak); Visit(b, 2, peak);
    int ok = b.spawned == 2 && b.alive == 0 && peak == 1;
    b.alive = 3; Visit(b, 1, peak); ok = ok && b.spawned == 2 && b.alive == 3;
    b.alive = 0; b.mazeDefeated = true; Visit(b, 1, peak); ok = ok && b.spawned == 2;
    if (!ok) { std::printf("00e1bc70_TEST FAIL\n"); return 1; }
    std::printf("00e1bc70_TEST PASS\n");
    return 0;
}