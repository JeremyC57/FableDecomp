#include <cstdio>
// Behaviour model of CHobbeLarder::Main (0x00D8F6D0): scripted-entity flow over a simulated frame clock.
struct World { int frame; int gruntFrame; int awareFrame; int terminateFrame; int timer; int rearms; int attacks; int sacrificeDies; int closeFrame; bool closeby; };
static bool Terminating(World& w) { return w.frame >= w.terminateFrame; }
static void NewFrame(World& w) { ++w.frame; if (w.timer > 0) --w.timer; }
static void Main(World& w, long alarmTime)
{
    NewFrame(w);
    if (Terminating(w)) return;
    while (w.frame < w.gruntFrame) {
        NewFrame(w);
        if (Terminating(w)) return;
        if (w.frame >= w.awareFrame && w.timer == 0) { if (Terminating(w)) return; w.timer = alarmTime; ++w.rearms; }
    }
    if (Terminating(w)) return;
    ++w.attacks;
    while (w.frame < w.sacrificeDies) {
        NewFrame(w);
        if (Terminating(w)) return;
        if (!w.closeby && w.frame >= w.closeFrame) { if (Terminating(w)) return; w.closeby = true; }
    }
    if (Terminating(w)) return;
}
int main()
{
    World a = { 0, 20, 5, 100, 0, 0, 0, 30, 25, false }; Main(a, 4);
    int ok = a.rearms == 4 && a.attacks == 1 && a.closeby && a.frame == 30;
    World b = { 0, 20, 5, 12, 0, 0, 0, 30, 25, false }; Main(b, 4);
    ok = ok && b.attacks == 0 && !b.closeby && b.frame == 12;
    if (!ok) { std::printf("00d8f6d0_TEST FAIL\n"); return 1; }
    std::printf("00d8f6d0_TEST PASS\n");
    return 0;
}