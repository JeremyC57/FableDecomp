// Behaviour test for CDarkwoodAssassinSpawn::Main (0x00e02e50): the ambush fires once the hero has stayed within the spawn
// distance while the trigger is off screen for the whole timer (2 ticks), then the assassin targets the hero (rand%5==0) or
// the trader, closes in through the 18/12/8 stages with a shout at each, and the trigger is removed. Timers are always freed.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt;
    int nearFrom; int offscreenFrom;          // hero within distance from frame nearFrom; trigger off screen from offscreenFrom
    int timer; int timersLive;
    int spawned; int targetHero; int targetTrader; int shouts; int removed;
    int assassinDist; int randValue;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.timer > 0) --s.timer; if (s.spawned) s.assassinDist -= 3; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    ++s.timersLive; s.timer = 2;
    for (;;) {
        if (Term(s)) { --s.timersLive; return; }
        if (s.frame >= s.nearFrom && s.frame >= s.offscreenFrom) {
            if (Term(s)) { --s.timersLive; return; }
        } else {
            if (Term(s)) { --s.timersLive; return; }
            s.timer = 2;
        }
        if (s.timer == 0) break;
        Frame(s);
    }
    if (Term(s)) { --s.timersLive; return; }
    ++s.spawned;
    if (s.randValue % 5 == 0) ++s.targetHero; else ++s.targetTrader;
    const int stages[3] = { 18, 12, 8 };
    for (int i = 0; i < 3; ++i) {
        if (s.assassinDist > stages[i]) {
            if (Term(s)) { --s.timersLive; return; }
            while (!(s.assassinDist < stages[i])) { Frame(s); if (Term(s)) { --s.timersLive; return; } }
            if (Term(s)) { --s.timersLive; return; }
            ++s.shouts;
        }
    }
    ++s.removed;
    --s.timersLive;
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 200; a.nearFrom = 5; a.offscreenFrom = 10; a.assassinDist = 30; a.randValue = 10;
    Sim b = a; b.randValue = 7; b.assassinDist = 10;
    Sim c = a; c.terminateAt = 8;                 // never spawns
    Main(a); Main(b); Main(c);
    bool ok = a.spawned == 1 && a.targetHero == 1 && a.shouts == 3 && a.removed == 1 && a.timersLive == 0
           && b.spawned == 1 && b.targetTrader == 1 && b.shouts == 1 && b.removed == 1 && b.timersLive == 0
           && c.spawned == 0 && c.removed == 0 && c.timersLive == 0;
    std::printf(ok ? "00e02e50_TEST PASS\n" : "00e02e50_TEST FAIL\n");
    return ok ? 0 : 1;
}
