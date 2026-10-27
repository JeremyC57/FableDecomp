// Behaviour test for vector<CCombatRing>::erase (0x006F45A0): shifts later rings down, destroys the old last ring's prongs, returns position.
#include <cstdio>
#include <cstdlib>
static int g_prongsDestroyed = 0;
struct Prong { bool Blocked; int Timer; ~Prong() { ++g_prongsDestroyed; } };
struct Ring {
    float RotationAngle; int Index;
    Prong* PStart; Prong* PFinish; Prong* PEos;
    void destroy() { for (Prong* p = PStart; p != PFinish; ++p) p->~Prong(); if (PStart) std::free(PStart); }
    void assign(const Ring& o)
    {
        RotationAngle = o.RotationAngle; Index = o.Index;
        int n = (int)(o.PFinish - o.PStart);
        destroy();
        PStart = n ? (Prong*)std::malloc(n * sizeof(Prong)) : 0;
        for (int i = 0; i < n; ++i) { PStart[i].Blocked = o.PStart[i].Blocked; PStart[i].Timer = o.PStart[i].Timer; }
        PFinish = PEos = PStart + n;
    }
};
struct RingVector {
    Ring* start; Ring* finish;
    Ring* erase(Ring* position)
    {
        if (position + 1 != finish)
            for (Ring* p = position; p + 1 != finish; ++p) p->assign(p[1]);
        --finish;
        finish->destroy();
        return position;
    }
};
static void make(Ring& r, int index, int prongs)
{
    r.Index = index; r.RotationAngle = (float)index;
    r.PStart = prongs ? (Prong*)std::malloc(prongs * sizeof(Prong)) : 0;
    for (int i = 0; i < prongs; ++i) { r.PStart[i].Blocked = false; r.PStart[i].Timer = index * 10 + i; }
    r.PFinish = r.PEos = r.PStart + prongs;
}
int main()
{
    Ring rings[3]; make(rings[0], 0, 1); make(rings[1], 1, 2); make(rings[2], 2, 3);
    RingVector v; v.start = rings; v.finish = rings + 3;
    Ring* r = v.erase(rings + 1);
    if (r != rings + 1 || v.finish != rings + 2 || rings[1].Index != 2 || rings[1].PFinish - rings[1].PStart != 3 || rings[1].PStart[2].Timer != 22) { std::printf("006f45a0_TEST FAIL\n"); return 1; }
    int before = g_prongsDestroyed;
    r = v.erase(rings + 1);
    if (r != rings + 1 || v.finish != rings + 1 || g_prongsDestroyed - before != 3 || rings[0].Index != 0) { std::printf("006f45a0_TEST FAIL\n"); return 1; }
    rings[0].destroy();
    std::printf("006f45a0_TEST PASS\n");
    return 0;
}