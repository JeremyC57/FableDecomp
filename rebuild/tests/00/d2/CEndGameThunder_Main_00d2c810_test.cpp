// Behaviour test for CEndGameThunder::Main (0x00d2c810): unless the fight is already over, Thunder walks to the hero, opens a
// conversation whose first line depends on the married flags; then, until the fight is over, every hero hit (plain, or a
// special ability other than 14) allies the pair, and a hero beyond 8 units is followed back to within 4.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int overAt;
    int dist; int strayFrame;                 // hero distance; at strayFrame the hero jumps to 10 units away
    bool married74, marriedGrey;
    int hitFrame; int hitAbility;             // a hit at hitFrame: ability 0 = plain hit, else special ability id
    int lines; int marriedLine; int allies; int moveTos; int releases; bool scripting;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static bool Over(const Sim& s) { return s.frame >= s.overAt; }
static void Frame(Sim& s) { ++s.frame; if (s.dist > 0) --s.dist; if (s.frame == s.strayFrame) s.dist = 10; }
static bool Hit(const Sim& s) { return s.frame == s.hitFrame && (s.hitAbility == 0 || s.hitAbility != 14); }
static void HitCheck(Sim& s, bool& quit)
{
    if (Hit(s)) { if (Term(s)) { quit = true; return; } s.allies += 2; }
}
static void Main(Sim& s)
{
    bool quit = false;
    if (!Over(s)) {
        if (Term(s)) return;
        s.scripting = true;
        ++s.moveTos;
        while (s.dist > 4) { Frame(s); if (Term(s)) return; }
        if (Term(s)) return;
        s.marriedLine = (!s.married74 && !s.marriedGrey) ? 1 : 2;
        s.lines += 3;
        if (s.scripting) { s.scripting = false; ++s.releases; }
    }
    while (!Over(s)) {
        Frame(s);
        if (Term(s)) return;
        HitCheck(s, quit); if (quit) return;
        if (s.dist > 8) {
            if (Term(s)) return;
            s.scripting = true; ++s.moveTos;
            while (s.dist > 4) {
                Frame(s);
                if (Term(s)) return;
                HitCheck(s, quit); if (quit) return;
                if (Over(s)) { if (Term(s)) return; if (s.scripting) { s.scripting = false; ++s.releases; } return; }
            }
            if (Term(s)) return;
            if (s.scripting) { s.scripting = false; ++s.releases; }
        }
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.overAt = 50; a.dist = 7; a.strayFrame = 20; a.hitFrame = 30; a.hitAbility = 0;
    Sim b = a; b.marriedGrey = true; b.hitAbility = 14;          // ability 14 does not count
    Sim c = a; c.overAt = 0;                                     // fight already over: nothing happens
    Main(a); Main(b); Main(c);
    bool ok = a.marriedLine == 1 && a.lines == 3 && a.allies == 2 && a.moveTos == 2 && a.releases == 2 && a.frame == 50
           && b.marriedLine == 2 && b.allies == 0 && b.moveTos == 2
           && c.lines == 0 && c.moveTos == 0 && c.frame == 0;
    std::printf(ok ? "00d2c810_TEST PASS\n" : "00d2c810_TEST FAIL\n");
    return ok ? 0 : 1;
}