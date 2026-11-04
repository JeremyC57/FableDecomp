// Behaviour test for CGuildmaster_HS::Main (0x00d85b40): while guards remain, speak the next of six lines (comments then
// taunts) whenever the hero is near and the 18-tick timer has run out; at most six lines; then heal to full health.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int csFrame; int guardsGoneFrame; int nearFrom; int timer;
    int spoken[7]; long comments; float health; int frameLimit;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.timer > 0) --s.timer; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (s.frame < s.csFrame) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    s.comments = 0;
    while (s.frame < s.guardsGoneFrame) {
        Frame(s);
        if (Term(s)) return;
        if (s.comments < 6 && s.timer == 0 && s.frame >= s.nearFrom) {
            if (Term(s)) return;
            switch (s.comments) {
            case 0: case 1: case 2: case 3: case 4: case 5: ++s.spoken[s.comments]; break;
            default: ++s.spoken[6]; break;
            }
            ++s.comments;
            s.timer = 18;
        }
    }
    if (Term(s)) return;
    s.health = s.health + (1.0f - s.health);
    do { Frame(s); } while (!Term(s));
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 400; a.csFrame = 5; a.guardsGoneFrame = 300; a.nearFrom = 10; a.health = 0.25f;
    Sim b = a; b.guardsGoneFrame = 40;          // guards gone after two lines
    Sim c = a; c.terminateAt = 50;              // terminates mid-fight: no heal
    Main(a); Main(b); Main(c);
    bool ok = a.comments == 6 && a.spoken[0] == 1 && a.spoken[5] == 1 && a.spoken[6] == 0 && a.health == 1.0f && a.frame == 400
           && b.comments == 2 && b.spoken[1] == 1 && b.spoken[2] == 0 && b.health == 1.0f
           && c.comments == 3 && c.health == 0.25f;
    std::printf(ok ? "00d85b40_TEST PASS\n" : "00d85b40_TEST FAIL\n");
    return ok ? 0 : 1;
}