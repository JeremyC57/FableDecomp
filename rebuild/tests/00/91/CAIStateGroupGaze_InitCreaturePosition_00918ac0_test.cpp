// Behaviour test for 0x00918AC0: InitCreaturePosition places the creature near a gaze marker on a <30 roll.
#include <cstdio>
static unsigned long ROR13(unsigned long v) { v &= 0xFFFFFFFFUL; return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }
static unsigned long Draw(unsigned long& seed, unsigned long range) { seed = (seed * 0x24A1 + 0x24DF) & 0xFFFFFFFFUL; seed = ROR13(seed); return range ? seed % range : 0; }
struct C3DVector { float X, Y, Z; };
struct CMarker { C3DVector pos; };
struct CGaze { bool runnable; unsigned long seed; CMarker* marker; const C3DVector* placedAt; float radius;
    bool AutoPlaceNear(const C3DVector& p, float r) { placedAt = &p; radius = r; return true; }
    bool Init()
    {
        if (runnable) {
            if (Draw(seed, 100) < 30.0f) { CMarker* m = marker; return AutoPlaceNear(m->pos, 1.0f); }
        }
        return false;
    }
};
int main()
{
    CMarker m = { { 1, 2, 3 } };
    int placed = 0, tries = 0, bad = 0;
    CGaze g = { true, 777, &m, 0, 0 };
    for (int i = 0; i < 1000; ++i) {
        unsigned long s = g.seed; unsigned long expect = Draw(s, 100);
        g.placedAt = 0;
        bool r = g.Init(); ++tries;
        if (r != (expect < 30)) bad = 1;
        if (r) { ++placed; if (g.placedAt != &m.pos || g.radius != 1.0f) bad = 2; }
    }
    if (placed < 200 || placed > 400) bad = 3;
    g.runnable = false; if (g.Init()) bad = 4;
    if (bad) { std::printf("00918ac0_TEST FAIL\n"); return 1; }
    std::printf("00918ac0_TEST PASS\n");
    return 0;
}