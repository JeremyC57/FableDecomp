// Behaviour test for CGuardOutsideWoods::Main (0x00d874f0): as the hero approaches, the warning cutscene plays once (within
// 35), then warnings escalate at 30 / 18 / 10 units; the third warning (or a hit by the hero, or the guards already attacking)
// makes the guard attack and idle.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; float distance; float step; int hitAt;
    bool giveCutscene; bool attack; long warnings; int cutscenes; int halts; int lines; bool attacked;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; s.distance -= s.step; }
static bool Near(const Sim& s, float d) { return s.distance < d; }
static void Attack(Sim& s) { s.attack = true; s.attacked = true; do Frame(s); while (!Term(s)); }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (!Term(s)) {
        if (s.giveCutscene == true && Near(s, 35.0f)) { s.giveCutscene = false; ++s.cutscenes; }
        switch (s.warnings) {
        case 0: if (Near(s, 30.0f)) { ++s.lines; ++s.warnings; ++s.halts; } break;
        case 1: if (Near(s, 18.0f)) { ++s.lines; ++s.warnings; ++s.halts; } break;
        case 2: if (Near(s, 10.0f)) { ++s.lines; Attack(s); return; } break;
        }
        if (s.frame == s.hitAt || s.attack) { Attack(s); return; }
        Frame(s);
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 500; a.distance = 50.0f; a.step = 1.0f; a.hitAt = -1; a.giveCutscene = true;
    Sim b = a; b.hitAt = 5;                       // hit before any warning
    Sim c = a; c.step = 0.0f; c.terminateAt = 50; // hero never approaches
    Main(a); Main(b); Main(c);
    bool ok = a.cutscenes == 1 && a.lines == 3 && a.halts == 2 && a.warnings == 2 && a.attacked && a.frame == 500
           && b.lines == 0 && b.attacked && b.cutscenes == 0
           && c.lines == 0 && !c.attacked && c.giveCutscene;
    std::printf(ok ? "00d874f0_TEST PASS\n" : "00d874f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
