// Behaviour test for CFireHeart::Main (0x00df2f90): wait for the quest flags, then while health > 1 play a heartbeat whose
// interval follows health (<=750 fast/1, <=1500 medium/2, else slow/3) and let the quest comment on summoner hits every 20 ticks;
// flag the heart destroyed when health drops to 1 or below.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int readyFrame; int activeFrame;
    float health; float damagePerFrame; int beatTimer; int commentTimer;
    int fast, medium, slow, comments, hitFrom;
    bool destroyed;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.beatTimer > 0) --s.beatTimer; if (s.commentTimer > 0) --s.commentTimer; if (s.frame > s.activeFrame) s.health -= s.damagePerFrame; }
static bool Hit(const Sim& s) { return s.frame >= s.hitFrom; }
static void Main(Sim& s)
{
    while (s.frame < s.readyFrame) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    while (s.frame < s.activeFrame) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    s.commentTimer = 0;
    s.beatTimer = 3;
    while (s.health > 1.0f) {
        Frame(s);
        if (Term(s)) return;
        if (s.beatTimer <= 0) {
            if (s.health <= 750.0f) { ++s.fast; s.beatTimer = 1; }
            else if (s.health <= 1500.0f) { ++s.medium; s.beatTimer = 2; }
            else { ++s.slow; s.beatTimer = 3; }
        }
        if (Hit(s) && s.commentTimer == 0) { ++s.comments; s.commentTimer = 20; }
    }
    if (Term(s)) return;
    s.destroyed = true;
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100000; a.readyFrame = 3; a.activeFrame = 6; a.health = 2000.0f; a.damagePerFrame = 20.0f; a.hitFrom = 1000000;
    Sim b = a; b.terminateAt = 30;                     // terminated mid-fight: never destroyed
    Sim c = a; c.hitFrom = 0;                          // hit every frame: comments every 20 ticks
    Main(a); Main(b); Main(c);
    bool ok = a.destroyed && a.slow > 0 && a.medium > 0 && a.fast > 0 && a.comments == 0
           && !b.destroyed && b.fast == 0
           && c.destroyed && c.comments >= 5 && c.comments <= 6;
    std::printf(ok ? "00df2f90_TEST PASS\n" : "00df2f90_TEST FAIL\n");
    return ok ? 0 : 1;
}