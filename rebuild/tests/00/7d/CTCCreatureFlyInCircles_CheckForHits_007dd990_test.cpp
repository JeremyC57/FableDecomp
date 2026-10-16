// behaviour test for 0x007DD990 CTCCreatureFlyInCircles::CheckForHits
#include <cstdio>
struct CThing { unsigned char flags91; };
struct HitBy { const CThing* who; HitBy(const CThing* w) : who(w) {} };
struct Shot { Shot() {} };
struct Filter { const HitBy& a; const Shot& b; Filter(const HitBy& x, const Shot& y) : a(x), b(y) {} };
struct Events { const CThing* hitter; int queries; bool GetEvent(int** out, const Filter& f) { ++queries; *out = 0; return f.a.who == hitter; } };
struct Creature {
    unsigned int bits[9]; Events* ev;
    template<class T> bool GetInterface(T** out, long type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = ev; return true; } return false; }
};
struct Fly {
    Creature* creature; bool HasBeenHit; CThing* hero;
    void CheckForHits()
    {
        if (!HasBeenHit) {
            CThing* h = hero; Events* events;
            if (h && !(h->flags91 & 1) && creature->GetInterface(&events, 0x42)) {
                int* event;
                if (events->GetEvent(&event, Filter(HitBy(h), Shot()))) HasBeenHit = true;
            }
        }
    }
};
int main()
{
    CThing hero = { 0 }, other = { 0 };
    Events ev = { &other, 0 };
    Creature c; for (int i = 0; i < 9; ++i) c.bits[i] = 0; c.ev = &ev;
    Fly f; f.creature = &c; f.HasBeenHit = false; f.hero = &hero;
    f.CheckForHits();
    bool ok = ev.queries == 0 && !f.HasBeenHit;           // no entity-events interface
    c.bits[2] = 4; f.CheckForHits();
    ok = ok && ev.queries == 1 && !f.HasBeenHit;
    ev.hitter = &hero; f.CheckForHits();
    ok = ok && ev.queries == 2 && f.HasBeenHit;
    f.CheckForHits();
    ok = ok && ev.queries == 2;
    f.HasBeenHit = false; hero.flags91 = 1; f.CheckForHits();
    ok = ok && ev.queries == 2 && !f.HasBeenHit;
    if (!ok) { std::printf("007dd990_TEST FAIL\n"); return 1; }
    std::printf("007dd990_TEST PASS\n");
    return 0;
}