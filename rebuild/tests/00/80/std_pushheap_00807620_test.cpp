// Behaviour test for 0x00807620 std::__push_heap<pair<C3DVector,long>*, int, pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting>:
// the row's STLport sift-up over a min-heap of targeting scores (comp(a,b) = score(a) > score(b)); the score is a stub
// (the X coordinate scaled by the comparator's AngleCompare), as GetCompareValueForPoint 0x007F9660 is a separate row.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C3DVector { public: float X, Y, Z; };
namespace std { template <class A, class B> struct pair { A first; B second; }; }
class CVectorCompare_BestForTargeting {
public:
    C3DVector AnchorPos, PreferredLine;
    float AngleCompare, SqrDistCompare, OverrideShortDistanceFactor, OverrideShortDistanceFalloff;
    bool CompareInXYPlane;
    float GetCompareValueForPoint(const C3DVector& p) const { ++Calls; return p.X * AngleCompare; }
    bool operator()(const C3DVector& a, const C3DVector& b) const
    {
        float aValue = (GetCompareValueForPoint(a));
        float bValue = (GetCompareValueForPoint(b));
        return aValue > bValue;
    }
    static int Calls;
};
int CVectorCompare_BestForTargeting::Calls = 0;
namespace {
class CCompareTargetedPositionsForFirstPersonTargeting {
public:
    CVectorCompare_BestForTargeting VectorCompare;
    bool operator()(const std::pair<C3DVector, long>& a, const std::pair<C3DVector, long>& b) const
    {
        return VectorCompare(a.first, b.first);
    }
};
}
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
static std::pair<C3DVector, long> P(float x, long id) { std::pair<C3DVector, long> p; p.first.X = x; p.first.Y = 2; p.first.Z = 3; p.second = id; return p; }
int main()
{
    CCompareTargetedPositionsForFirstPersonTargeting comp;
    comp.VectorCompare.AngleCompare = 2.0f;
    // min-heap on the score: 1, 3, 2, 5, 4, hole at 5 (parent 2 -> 0)
    std::pair<C3DVector, long> h[6] = { P(1, 10), P(3, 11), P(2, 12), P(5, 13), P(4, 14), P(0, 0) };
    std::__push_heap(h, 5, 0, P(1.5f, 99), comp);          // 1.5 < 2 at index 2 -> moves up to 2, stops under 1
    int ok = h[5].second == 12 && h[2].second == 99 && h[0].second == 10 && h[2].first.Y == 2 && h[2].first.Z == 3;
    CVectorCompare_BestForTargeting::Calls = 0;
    std::__push_heap(h, 5, 0, P(0.5f, 77), comp);          // hole 5 again: 0.5 beats 1.5 and 1 -> root
    ok = ok && h[0].second == 77 && h[2].second == 10 && h[5].second == 99 && CVectorCompare_BestForTargeting::Calls == 4;
    std::__push_heap(h, 4, 2, P(9.0f, 55), comp);          // parent 1 index below top? 4 > 2, parent 1: 3*2 > 18 false -> stays
    ok = ok && h[4].second == 55 && h[1].second == 11;
    std::__push_heap(h, 3, 3, P(-1.0f, 66), comp);         // hole == top: stored in place, no compare
    ok = ok && h[3].second == 66;
    puts(ok ? "00807620_TEST PASS" : "00807620_TEST FAIL");
    return ok ? 0 : 1;
}