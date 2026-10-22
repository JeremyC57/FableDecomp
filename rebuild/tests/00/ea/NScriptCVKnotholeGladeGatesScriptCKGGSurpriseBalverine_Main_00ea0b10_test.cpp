#include <cstdio>
// Behaviour model of CKGG_SurpriseBalverine::Main (0x00EA0B10): munch until hero near / hit / armed timer expiry.
struct World { int frame; int terminateFrame; int onScreenFrame; float heroDistance0; int heroApproach; int hitFrame; bool alive; int timer; int attacks; bool firstDead; };
static bool Terminating(World& w) { return w.frame >= w.terminateFrame; }
static float HeroDistance(World& w) { return w.heroDistance0 - (float)(w.frame * w.heroApproach); }
static void Main(World& w)
{
    ++w.frame;
    if (Terminating(w)) return;
    bool timerSet = false;
    while (!(HeroDistance(w) < 5.0f) && w.frame != w.hitFrame && (!timerSet || w.timer > 0)) {
        ++w.frame; if (w.timer > 0) --w.timer;
        if (Terminating(w)) return;
        if (!timerSet && w.frame >= w.onScreenFrame && HeroDistance(w) < 10.0f) { timerSet = true; w.timer = 8; }
    }
    ++w.frame;
    if (Terminating(w)) return;
    if (w.alive) { ++w.attacks; } else { w.firstDead = true; }
}
int main()
{
    World a = { 0, 1000, 0, 20.0f, 1, -1, true, 0, 0, false }; Main(a);
    int ok = a.attacks == 1 && !a.firstDead;
    World b = { 0, 1000, 0, 9.0f, 0, -1, true, 0, 0, false }; Main(b);
    ok = ok && b.attacks == 1 && b.frame >= 9;
    World c = { 0, 1000, 0, 100.0f, 0, 6, false, 0, 0, false }; Main(c);
    ok = ok && c.firstDead && c.attacks == 0;
    World d = { 0, 5, 0, 100.0f, 0, -1, true, 0, 0, false }; Main(d);
    ok = ok && d.attacks == 0 && !d.firstDead;
    if (!ok) { std::printf("00ea0b10_TEST FAIL\n"); return 1; }
    std::printf("00ea0b10_TEST PASS\n");
    return 0;
}