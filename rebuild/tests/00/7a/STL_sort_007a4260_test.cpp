// Behaviour test for 0x007A4260 std::sort<CThing**, CThingCompare_NearestToPos>: a non-empty range runs
// the introsort loop with depth 2*lg(n) and then the final insertion sort; an empty range does nothing.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CThing { C3DVector P; };
struct CThingCompare_NearestToPos {
    C3DVector Pos;
    bool operator()(const CThing* a, const CThing* b) const
    {
        float da = (a->P.X - Pos.X) * (a->P.X - Pos.X) + (a->P.Y - Pos.Y) * (a->P.Y - Pos.Y);
        float db = (b->P.X - Pos.X) * (b->P.X - Pos.X) + (b->P.Y - Pos.Y) * (b->P.Y - Pos.Y);
        return da < db;
    }
};

static int g_introCalls = 0, g_finalCalls = 0;
static long g_depth = -1;

static long Lg(long n) { long k; for (k = 0; n != 1; n >>= 1) ++k; return k; }

// stand-ins: the introsort loop records its depth, the final insertion sort sorts fully
static void IntrosortLoop(CThing** first, CThing** last, CThing**, long depth, CThingCompare_NearestToPos)
{
    ++g_introCalls; g_depth = depth; (void)first; (void)last;
}
static void FinalInsertionSort(CThing** first, CThing** last, CThingCompare_NearestToPos comp)
{
    ++g_finalCalls;
    for (CThing** i = first + 1; i < last; ++i) {
        CThing* v = *i; CThing** j = i;
        while (j > first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}
static void Sort(CThing** first, CThing** last, CThingCompare_NearestToPos comp)
{
    if (first != last) {
        IntrosortLoop(first, last, (CThing**)0, Lg((long)(last - first)) * 2, comp);
        FinalInsertionSort(first, last, comp);
    }
}

int main()
{
    CThing t[5];
    float xs[5] = { 9.0f, 1.0f, 5.0f, -3.0f, 2.0f };
    CThing* p[5];
    for (int i = 0; i < 5; ++i) { t[i].P.X = xs[i]; p[i] = &t[i]; }
    CThingCompare_NearestToPos comp;
    comp.Pos.X = 0.0f;
    Sort(p, p, comp);
    if (g_introCalls != 0 || g_finalCalls != 0) { std::printf("007a4260_TEST FAIL\n"); return 1; }
    Sort(p, p + 5, comp);
    if (g_introCalls != 1 || g_finalCalls != 1 || g_depth != 4) { std::printf("007a4260_TEST FAIL\n"); return 1; }
    if (p[0] != &t[1] || p[1] != &t[4] || p[2] != &t[3] || p[3] != &t[2] || p[4] != &t[0]) { std::printf("007a4260_TEST FAIL\n"); return 1; }
    std::printf("007a4260_TEST PASS\n");
    return 0;
}