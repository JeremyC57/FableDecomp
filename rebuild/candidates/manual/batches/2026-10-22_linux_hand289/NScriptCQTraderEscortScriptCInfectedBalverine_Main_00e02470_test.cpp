#include <cstdio>
// Behaviour model of the first CInfectedBalverine::Main (0x00E02470): munch on the dinner until the hero comes near or hits it.
struct World { int frame; int terminateFrame; int shownFrame; int nearFrame; int hitFrame; int taskEnd; int munches; int leaps; long counter; };
static bool Terminating(World& w) { return w.frame >= w.terminateFrame; }
static void Main(World& w)
{
    ++w.frame;
    if (Terminating(w)) return;
    while (w.frame < w.shownFrame) { ++w.frame; if (Terminating(w)) return; }
    if (Terminating(w)) return;
    bool heroNear = false, leave = false;
    while (!leave) {
        ++w.frame;
        if (Terminating(w)) return;
        if (!heroNear) {
            if (w.frame >= w.taskEnd) { ++w.munches; w.taskEnd = w.frame + 3; }
            if (w.frame >= w.nearFrame) {
                heroNear = true;
                while (w.frame < w.taskEnd) { ++w.frame; if (Terminating(w)) return; }
                leave = true;
            }
        }
        if (w.frame == w.hitFrame) break;
    }
    if (Terminating(w)) return;
    ++w.leaps;
    ++w.counter;
}
int main()
{
    World a = { 0, 100, 5, 12, -1, 0, 0, 0, 0 }; Main(a);
    int ok = a.leaps == 1 && a.counter == 1 && a.munches >= 2 && a.frame >= 12;
    World b = { 0, 100, 5, 1000, 9, 0, 0, 0, 0 }; Main(b);
    ok = ok && b.leaps == 1 && b.frame == 9;
    World c = { 0, 4, 5, 1000, -1, 0, 0, 0, 0 }; Main(c);
    ok = ok && c.leaps == 0 && c.munches == 0;
    if (!ok) { std::printf("00e02470_TEST FAIL\n"); return 1; }
    std::printf("00e02470_TEST PASS\n");
    return 0;
}
