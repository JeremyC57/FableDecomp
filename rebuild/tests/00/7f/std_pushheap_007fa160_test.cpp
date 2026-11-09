// Behaviour test for 0x007FA160 std::__push_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2>: STLport sift-up
// with the comparator's operator() inlined (both multipliers computed and discarded, b's position first, then valueA > valueB).
// GetThingCompareMultiplier / GetPos / GetCompareValueForPoint are stubs (separate rows); the score is the position's X.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C3DVector { public: float X, Y, Z; };
class CThing { public: C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
static int g_multiplierCalls = 0, g_valueCalls = 0;
class CVectorCompare_BestForTargeting {
public:
    unsigned char _body[0x2c];
    float GetCompareValueForPoint(const C3DVector& p) const { ++g_valueCalls; return p.X; }
};
class CThingCompare_BestForTargeting2 {
public:
    CThing* Hero;
    CVectorCompare_BestForTargeting VectorCompare;
    float EnemyPreferenceFactor;
    float GetThingCompareMultiplier(CThing*) { ++g_multiplierCalls; return EnemyPreferenceFactor; }
    bool operator()(CThing* a, CThing* b)
    {
        float multiplierA = GetThingCompareMultiplier(a);
        float multiplierB = GetThingCompareMultiplier(b);
        (void)multiplierA; (void)multiplierB;
        const C3DVector& posB = b->GetPos();
        const C3DVector& posA = a->GetPos();
        float valueA = VectorCompare.GetCompareValueForPoint(posA);
        float valueB = VectorCompare.GetCompareValueForPoint(posB);
        if (valueA > valueB)
            return true;
        return false;
    }
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}
int main()
{
    CThing t[8];
    float xs[8] = { 1, 3, 2, 5, 4, 0, 1.5f, 0.5f };
    for (int i = 0; i < 8; ++i) { t[i].Pos.X = xs[i]; t[i].Pos.Y = t[i].Pos.Z = 0; }
    CThingCompare_BestForTargeting2 comp; comp.Hero = 0; comp.EnemyPreferenceFactor = 1.0f;
    CThing* h[6] = { &t[0], &t[1], &t[2], &t[3], &t[4], 0 };
    std::__push_heap(h, 5, 0, &t[6], comp);                     // 1.5: above 2 at index 2, below 1 at the root
    int ok = h[5] == &t[2] && h[2] == &t[6] && h[0] == &t[0] && g_multiplierCalls == 4 && g_valueCalls == 4;
    std::__push_heap(h, 5, 0, &t[7], comp);                     // 0.5 rises to the root
    ok = ok && h[0] == &t[7] && h[2] == &t[0] && h[5] == &t[6];
    std::__push_heap(h, 3, 3, &t[5], comp);                     // hole == top: no compare
    ok = ok && h[3] == &t[5] && g_valueCalls == 8;
    puts(ok ? "007fa160_TEST PASS" : "007fa160_TEST FAIL");
    return ok ? 0 : 1;
}