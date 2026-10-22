#include <cstdio>
// Behaviour model of CLeader::Main (0x00EF3F00): walk Marker1..Marker6 in turn, re-taking scripting control for each leg;
// every exit (wait-loop return, post-wait return, end of leg) must release the target and the handle exactly once.
struct World { int frame; int terminateFrame; int startFailures; int live; int legs; int visits[8]; };
static bool Terminating(World& w) { return w.frame >= w.terminateFrame; }
static bool StartScripting(World& w) { if (w.startFailures > 0) { --w.startFailures; return false; } return true; }
static void Main(World& w)
{
    ++w.frame;
    if (Terminating(w)) return;
    long marker = 1;
    while (!Terminating(w)) {
        ++w.live;                                   // target
        ++w.live;                                   // handle
        while (!StartScripting(w)) {
            ++w.frame;
            if (Terminating(w)) { w.live -= 2; return; }
        }
        if (Terminating(w)) { w.live -= 2; return; }
        ++w.visits[marker];
        ++w.legs;
        if (++marker > 6) marker = 1;
        w.live -= 2;
        ++w.frame;
    }
}
int main()
{
    World a = { 0, 15, 0, 0, 0, { 0 } }; Main(a);
    int ok = a.live == 0 && a.legs == 14 && a.visits[1] == 3 && a.visits[2] == 3 && a.visits[3] == 2 && a.visits[6] == 2 && a.visits[0] == 0 && a.visits[7] == 0;
    World b = { 0, 5, 100, 0, 0, { 0 } }; Main(b);
    ok = ok && b.live == 0 && b.legs == 0;
    World c = { 0, 1, 0, 0, 0, { 0 } }; Main(c);
    ok = ok && c.live == 0 && c.legs == 0;
    if (!ok) { std::printf("00ef3f00_TEST FAIL\n"); return 1; }
    std::printf("00ef3f00_TEST PASS\n");
    return 0;
}