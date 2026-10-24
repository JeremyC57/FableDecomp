// Behaviour test for 0x008074A0 UnguardedLinearInsert_PairVectorLong_008074a0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct C3DVector {
    float X, Y, Z;
};

struct PairVectorLong {                // std::pair<C3DVector, long>, 16 bytes
    C3DVector first;                   // +0x00
    long      second;                  // +0x0C
};

struct CVectorCompare_BestForTargeting {
    unsigned char _body[0x2c];
    float GetCompareValueForPoint(const C3DVector& point);     // 0x007F9660
};

struct CCompareTargetedPositionsForFirstPersonTargeting {      // anonymous namespace in retail, 0x2c bytes
    CVectorCompare_BestForTargeting VectorCompare;             // +0x00
    bool operator()(const PairVectorLong& a, const PairVectorLong& b)
    {
        return VectorCompare.GetCompareValueForPoint(a.first) > VectorCompare.GetCompareValueForPoint(b.first);
    }
};

void __fastcall UnguardedLinearInsert_PairVectorLong_008074a0(PairVectorLong* last, PairVectorLong val, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    PairVectorLong* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

float CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector& point) { return point.X; }

typedef PairVectorLong T;
#define KEY(E) (E).second
#define INIT(E) (void)0
#define CALL(f, l, r) UnguardedLinearInsert_PairVectorLong_008074a0(f, l, r, tag, 0)

int main()
{
    int i;
    
    PairVectorLong a[5];
    float xs[5] = { 9.0f, 7.0f, 5.0f, 3.0f, 6.0f };
    for (i = 0; i < 5; ++i) { a[i].first.X = xs[i]; a[i].first.Y = 0.0f; a[i].first.Z = 0.0f; a[i].second = i; }
    CCompareTargetedPositionsForFirstPersonTargeting comp;
    memset(&comp, 0, sizeof(comp));
    UnguardedLinearInsert_PairVectorLong_008074a0(a + 4, a[4], comp);
    CHECK(a[0].first.X == 9.0f && a[1].first.X == 7.0f && a[2].first.X == 6.0f && a[3].first.X == 5.0f && a[4].first.X == 3.0f);
    CHECK(a[2].second == 4 && a[3].second == 2 && a[4].second == 3);
    a[4].first.X = 1.0f; a[4].second = 9;
    UnguardedLinearInsert_PairVectorLong_008074a0(a + 4, a[4], comp);
    CHECK(a[4].first.X == 1.0f && a[4].second == 9 && a[3].second == 2);
    if (g_fail) { std::printf("008074a0_TEST FAIL\n"); return 1; }
    std::printf("008074a0_TEST PASS\n");
    return 0;
}