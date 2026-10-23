// Behaviour test for CGuardNotThisWay::Main (0x00da68e0): each time the 15-tick timer is out and the hero is within 12
// units, the guard points (animation), says its two "not this way" lines, points again and restarts the timer.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int grantFrom; int nearFrom; int nearTo; int timer;
    int points; int lines; int conversations; int animFrames; bool attached;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.timer > 0) --s.timer; }
static bool Busy(Sim& s) { if (s.animFrames > 0) { --s.animFrames; return true; } return false; }
static bool Point(Sim& s)
{
    ++s.points; s.animFrames = 3;
    while (Busy(s)) { Frame(s); if (Term(s)) return false; }
    if (Term(s)) return false;
    return true;
}
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (s.frame < s.grantFrom) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    s.attached = true;
    s.timer = 0;
    for (;;) {
        if (Term(s)) return;
        if (s.timer == 0 && s.frame >= s.nearFrom && s.frame < s.nearTo) {
            if (Term(s)) return;
            if (!Point(s)) return;
            ++s.conversations; s.lines += 2;
            if (!Point(s)) return;
            s.timer = 15;
        }
        Frame(s);
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.grantFrom = 3; a.nearFrom = 10; a.nearTo = 60;
    Sim b = a; b.terminateAt = 12;              // terminates during the first pointing animation
    Main(a); Main(b);
    bool ok = a.attached && a.conversations == 3 && a.lines == 6 && a.points == 6 && a.frame == 100
           && b.points == 1 && b.conversations == 0;
    std::printf(ok ? "00da68e0_TEST PASS\n" : "00da68e0_TEST FAIL\n");
    return ok ? 0 : 1;
}