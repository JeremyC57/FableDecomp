#include <cstdio>
// Behaviour model of CTCSummonSpell::CancelMove (0x0079A820): a live minion with the timer component has it stopped once
// (current = target, stopped flag set); a stopped timer is left alone.
struct Timer { bool stopped; long current, target; };
struct Minion { bool dead; Timer* timer; };
static void CancelMove(Minion* m)
{
    if (m && !m->dead && m->timer) {
        Timer* t = m->timer;
        if (!t->stopped) { t->current = t->target; t->stopped = true; }
    }
}
int main()
{
    Timer t = { false, 1, 9 }, s = { true, 1, 9 };
    Minion a = { false, &t }, b = { false, &s }, c = { true, &t };
    CancelMove(&a); CancelMove(&b); CancelMove(0);
    t.current = 5; CancelMove(&c);
    if (!t.stopped || t.current != 5 || s.current != 1) { std::printf("0079a820_TEST FAIL\n"); return 1; }
    std::printf("0079a820_TEST PASS\n");
    return 0;
}
