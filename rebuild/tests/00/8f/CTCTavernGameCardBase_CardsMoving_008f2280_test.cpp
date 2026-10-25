// Behaviour test for 0x008F2280 CTCTavernGameCardBase::CardsMoving()
// The model mirrors the source: while any pack card is still moving nothing happens. State 20 goes to 21, 27 to 6 and
// any other state but 26 to 0. State 26 sends every dealt card (Cards) to the pack position at height
// (count - i) * CardSeparation + TableSeparation, removes that many cards from the front of the pack, sends the remaining
// pack cards to heights 53, 52, ... and appends the dealt cards to the pack, then goes to 27. Move restarts a moving card
// from CurrentPos or starts a resting one from LocalPos; the target goes to NewLocalPos with MoveFraction 0.
#include <cstdio>

struct Vec { float X, Y, Z; };
struct Card {
    bool Moving;
    float MoveFraction;
    Vec LocalPos, NewLocalPos, CurrentPos;
    int id;
    void Move(const Vec& position)
    {
        if (Moving)
            LocalPos = CurrentPos;
        else {
            CurrentPos = LocalPos;
            Moving = true;
        }
        NewLocalPos = position;
        MoveFraction = 0.0f;
    }
};
struct Def { float CardWidth, CardHeight, CardSeparation, TableSeparation, PackOffsetX, PackOffsetY; };
static int g_defRefs = 0;

struct Game {
    long state;
    int setStateCalls;
    Card* cards[8]; unsigned ncards;
    Card* pack[16]; unsigned npack;
    Def def;
    void SetState(long s) { state = s; ++setStateCalls; }
    Vec GetPackPosition() { ++g_defRefs; Vec v = { def.CardWidth * def.PackOffsetX, def.CardHeight * def.PackOffsetY, 0.0f }; --g_defRefs; return v; }
    void CardsMoving()
    {
        ++g_defRefs;
        for (unsigned k = 0; k < npack; ++k)
            if (pack[k]->Moving == true) { --g_defRefs; return; }
        if (state == 20) { SetState(21); --g_defRefs; return; }
        if (state == 26) {
            Vec pp = GetPackPosition();
            for (unsigned i = 0; i < ncards; ++i) {
                Vec p = { pp.X, pp.Y, (ncards - i) * def.CardSeparation + def.TableSeparation };
                cards[i]->Move(p);
            }
            for (unsigned j = 0; j < ncards; ++j) {
                for (unsigned k = 1; k < npack; ++k) pack[k - 1] = pack[k];
                --npack;
            }
            long i = 0;
            for (unsigned k = 0; k < npack; ++k, ++i) {
                Vec p = { pp.X, pp.Y, (53 - i) * def.CardSeparation + def.TableSeparation };
                pack[k]->Move(p);
            }
            for (unsigned c = 0; c < ncards; ++c) pack[npack++] = cards[c];
            SetState(27);
            --g_defRefs;
            return;
        }
        if (state == 27) { SetState(6); --g_defRefs; return; }
        SetState(0);
        --g_defRefs;
    }
};

int main()
{
    int fail = 0;
    Card c[6];
    for (int k = 0; k < 6; ++k) {
        c[k].Moving = false; c[k].MoveFraction = 0.5f; c[k].id = k;
        Vec lp = { (float)k, 0.0f, 0.0f }; c[k].LocalPos = lp; c[k].NewLocalPos = lp;
        Vec cp = { (float)k, 1.0f, 0.0f }; c[k].CurrentPos = cp;
    }
    Game g;
    g.state = 20; g.setStateCalls = 0;
    g.ncards = 2; g.cards[0] = &c[4]; g.cards[1] = &c[5];
    g.npack = 4; g.pack[0] = &c[0]; g.pack[1] = &c[1]; g.pack[2] = &c[2]; g.pack[3] = &c[3];
    Def d = { 2.0f, 3.0f, 0.5f, 10.0f, 1.0f, 4.0f }; g.def = d;
    c[2].Moving = true;
    g.CardsMoving();                                             // a pack card still moving: nothing
    if (g.state != 20 || g.setStateCalls != 0) fail |= 1;
    c[2].Moving = false;
    g.CardsMoving();
    if (g.state != 21 || g.setStateCalls != 1) fail |= 2;
    g.state = 27; g.CardsMoving();
    if (g.state != 6) fail |= 4;
    g.state = 5; g.CardsMoving();
    if (g.state != 0) fail |= 8;
    g.state = 26;
    c[5].Moving = true;                                          // a dealt card already moving restarts from CurrentPos
    g.CardsMoving();
    if (g.state != 27) fail |= 16;
    if (c[4].NewLocalPos.X != 2.0f || c[4].NewLocalPos.Y != 12.0f || c[4].NewLocalPos.Z != 2 * 0.5f + 10.0f) fail |= 32;
    if (c[5].NewLocalPos.Z != 1 * 0.5f + 10.0f || c[5].LocalPos.Y != 1.0f || c[4].CurrentPos.Y != 0.0f || !c[4].Moving) fail |= 64;
    if (g.npack != 4 || g.pack[0] != &c[2] || g.pack[1] != &c[3] || g.pack[2] != &c[4] || g.pack[3] != &c[5]) fail |= 128;
    if (c[2].NewLocalPos.Z != 53 * 0.5f + 10.0f || c[3].NewLocalPos.Z != 52 * 0.5f + 10.0f || c[2].MoveFraction != 0.0f) fail |= 256;
    if (c[0].Moving || c[1].Moving) fail |= 512;                 // the dropped front cards are not moved
    if (g_defRefs != 0) fail |= 1024;
    if (fail) { std::printf("008f2280_TEST FAIL %d\n", fail); return 1; }
    std::printf("008f2280_TEST PASS\n");
    return 0;
}