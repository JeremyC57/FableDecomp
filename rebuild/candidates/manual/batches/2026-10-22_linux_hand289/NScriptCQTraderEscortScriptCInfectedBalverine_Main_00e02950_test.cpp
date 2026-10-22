#include <cstdio>
// Behaviour model of the second CInfectedBalverine::Main (0x00E02950): munch until the hero is near, hits it, or the mission fails.
struct World { int frame; int terminateFrame; int nearFrame; int hitFrame; int failFrame; long range; int munches; int clears; bool canGetUp; int leaps; int removed; };
static bool Terminating(World& w) { return w.frame >= w.terminateFrame; }
static void Main(World& w)
{
    ++w.frame;
    if (Terminating(w)) return;
    bool done = false;
    float range = (float)w.range;
    while (!done) {
        ++w.frame;
        if (Terminating(w)) return;
        if (w.frame % 3 == 0) ++w.munches;
        float distance = w.frame >= w.nearFrame ? 1.0f : 100.0f;
        if (distance < range) { ++w.clears; done = true; }
        if (w.frame == w.hitFrame) done = true;
        if (w.frame >= w.failFrame) break;
    }
    if (Terminating(w)) return;
    w.canGetUp = true;
    ++w.leaps;
    ++w.removed;
}
int main()
{
    World a = { 0, 100, 10, -1, 1000, 5, 0, 0, false, 0, 0 }; Main(a);
    int ok = a.clears == 1 && a.canGetUp && a.removed == 1 && a.frame == 10;
    World b = { 0, 100, 1000, 7, 1000, 5, 0, 0, false, 0, 0 }; Main(b);
    ok = ok && b.clears == 0 && b.canGetUp && b.frame == 7;
    World c = { 0, 100, 1000, -1, 4, 5, 0, 0, false, 0, 0 }; Main(c);
    ok = ok && c.canGetUp && c.frame == 4;
    World d = { 0, 6, 1000, -1, 1000, 5, 0, 0, false, 0, 0 }; Main(d);
    ok = ok && !d.canGetUp && d.removed == 0;
    if (!ok) { std::printf("00e02950_TEST FAIL\n"); return 1; }
    std::printf("00e02950_TEST PASS\n");
    return 0;
}
