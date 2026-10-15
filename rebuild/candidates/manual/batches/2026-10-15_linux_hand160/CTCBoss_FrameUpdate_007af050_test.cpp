// Behaviour test for CTCBoss::FrameUpdate (0x007AF050): while strikes are counted, a melee hit on the live hero by this boss
// (found through the hero's entity events with the Bind2 filter) resets the count.
#include <cstdio>
struct Thing;
struct Melee { Melee() {} };
struct HitBy { Thing* Attacker; HitBy(Thing* a) : Attacker(a) {} };
struct Filter { const Melee* PA; const HitBy* PB; Filter(const Melee& a, const HitBy& b) : PA(&a), PB(&b) {} };
struct Event { int id; };
static Thing* g_lastAttacker; static Event g_event;
struct Events { bool GetEvent(const Event** out, const Filter& f) { *out = &g_event; return f.PB->Attacker == g_lastAttacker; } };
struct Thing {
    unsigned char Flags91; unsigned int bits[9]; Events* ev;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    bool GetInterface(Events** o, int t) { if (bits[t >> 5] & (1u << (t & 31))) { *o = ev; return true; } return false; }
};
static Thing* g_hero;
struct Boss {
    Thing* ParentThing; long StrikesSinceHit;
    void FrameUpdate()
    {
        if (StrikesSinceHit > 0) {
            Thing* hero = g_hero; Events* events;
            if (hero && !hero->IsDead() && hero->GetInterface(&events, 0x42)) {
                const Event* hit;
                if (events->GetEvent(&hit, Filter(Melee(), HitBy(ParentThing)))) StrikesSinceHit = 0;
            }
        }
    }
};
int main()
{
    Events ev; Thing hero; hero.Flags91 = 0; for (int i = 0; i < 9; ++i) hero.bits[i] = 0; hero.bits[2] = 1u << 2; hero.ev = &ev; g_hero = &hero;
    Thing bossThing; Boss b; b.ParentThing = &bossThing; b.StrikesSinceHit = 3;
    g_lastAttacker = 0; b.FrameUpdate();
    if (b.StrikesSinceHit != 3) { std::printf("007af050_TEST FAIL\n"); return 1; }
    g_lastAttacker = &bossThing; hero.Flags91 = 1; b.FrameUpdate();
    if (b.StrikesSinceHit != 3) { std::printf("007af050_TEST FAIL\n"); return 1; }
    hero.Flags91 = 0; b.FrameUpdate();
    if (b.StrikesSinceHit != 0) { std::printf("007af050_TEST FAIL\n"); return 1; }
    std::printf("007af050_TEST PASS\n");
    return 0;
}
