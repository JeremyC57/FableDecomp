#include <cstdio>
// Behaviour model of CHobbeLeader::Main (0x00D8FA30): scripted-entity flow over a simulated frame clock.
struct World { int frame; int startFrame; int awareFrame; int hitFrame; int terminateFrame; bool grunted; int sounds; int attacks; int timer; };
static bool Terminating(World& w) { return w.frame >= w.terminateFrame; }
static void NewFrame(World& w) { ++w.frame; if (w.timer > 0) --w.timer; }
static void Main(World& w)
{
    NewFrame(w);
    if (Terminating(w)) return;
    while (w.frame < w.startFrame) { NewFrame(w); if (Terminating(w)) return; }
    if (Terminating(w)) return;
    while (w.frame < w.awareFrame) { NewFrame(w); if (Terminating(w)) return; }
    if (Terminating(w)) return;
    if (!w.grunted) { if (Terminating(w)) return; ++w.sounds; w.grunted = true; }
    w.timer = 8;
    while (w.timer > 0) {
        NewFrame(w);
        if (Terminating(w)) return;
        if (w.frame == w.hitFrame) { if (Terminating(w)) return; break; }
    }
    if (Terminating(w)) return;
    ++w.attacks;
    do { NewFrame(w); } while (!Terminating(w));
}
int main()
{
    World a = { 0, 2, 4, 6, 100, false, 0, 0, 0 }; Main(a);
    int ok = a.sounds == 1 && a.attacks == 1 && a.frame == 100;
    World b = { 0, 2, 4, -1, 100, true, 0, 0, 0 }; Main(b);
    ok = ok && b.sounds == 0 && b.attacks == 1;
    World c = { 0, 2, 50, -1, 10, false, 0, 0, 0 }; Main(c);
    ok = ok && c.sounds == 0 && c.attacks == 0 && c.frame == 10;
    if (!ok) { std::printf("00d8fa30_TEST FAIL\n"); return 1; }
    std::printf("00d8fa30_TEST PASS\n");
    return 0;
}
