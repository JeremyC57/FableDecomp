// Behaviour test for CHS_Screamer::Main (0x00d83fa0): with no super screamer, an expired timer and the hero over 5 units
// away, this screamer becomes the super screamer; once the hero is within 15 units it drains the hero every timer period,
// lowers the mother's bar (by 0.05 while above the increment) and starts its super attack once; on the soul's collection
// it plays its death effect and is removed.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int soulFrame; float heroDistance; int timer; bool gotSuper; bool superScreamer;
    float bar; float inc; int damagePeriod; int drains; int attacks; int markers; int deathEffects; bool removed; int effects;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static bool Soul(const Sim& s) { return s.frame >= s.soulFrame; }
static void Frame(Sim& s) { ++s.frame; if (s.timer > 0) --s.timer; if (s.frame == 20) s.heroDistance = 10.0f; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (!Soul(s)) {
        Frame(s);
        if (Term(s)) return;
        if (!s.gotSuper && s.heroDistance > 5.0f) {
            if (Term(s)) return;
            s.gotSuper = true; s.superScreamer = true; ++s.markers;
            while (!(s.heroDistance < 15.0f || Soul(s))) { Frame(s); if (Term(s)) return; }
            if (Term(s)) return;
            if (!Soul(s)) {
                if (Term(s)) return;
                ++s.effects;
                int timer = s.damagePeriod;
                bool attacking = false;
                for (;;) {
                    if (Term(s)) return;
                    if (Soul(s)) break;
                    if (timer == 0) {
                        if (Term(s)) return;
                        ++s.drains; timer = 1;
                        if (s.bar > s.inc) { if (Term(s)) return; s.bar -= 0.05f; }
                        if (!attacking) { if (Term(s)) return; ++s.attacks; attacking = true; }
                    }
                    Frame(s); if (timer > 0) --timer;
                }
                if (Term(s)) return;
            }
        }
    }
    if (Term(s)) return;
    ++s.deathEffects; s.removed = true;
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 1000; a.soulFrame = 60; a.heroDistance = 30.0f; a.bar = 1.0f; a.inc = 0.5f; a.damagePeriod = 3;
    Sim b = a; b.terminateAt = 25;
    Main(a); Main(b);
    bool ok = a.superScreamer && a.markers == 1 && a.effects == 1 && a.attacks == 1 && a.drains > 5 && a.bar < 1.0f
           && a.deathEffects == 1 && a.removed
           && b.attacks == 1 && !b.removed && b.deathEffects == 0;
    std::printf(ok ? "00d83fa0_TEST PASS\n" : "00d83fa0_TEST FAIL\n");
    return ok ? 0 : 1;
}
