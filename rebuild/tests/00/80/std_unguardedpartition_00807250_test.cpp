// Behaviour test for 0x00807250 std::__unguarded_partition<pair<C3DVector,long>*, ..., CCompareTargetedPositionsForFirstPersonTargeting>
// (C++03). The model is the row's: comp(a, b) = value(a.first) > value(b.first); the partition moves every element that
// scores better than the pivot in front of the returned cut, swapping whole pairs.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;
    // test stand-in for 0x007F9660: a distance-like score (lower X+Y is nearer, but the comparator ranks larger first)
    float GetCompareValueForPoint(const C3DVector& p) const { return (p.X - AnchorPos.X) + 0.5f * (p.Y - AnchorPos.Y); }
};
struct PosEntry {
    C3DVector first;
    long second;
    PosEntry() { }
    PosEntry(const PosEntry& o) : first(o.first), second(o.second) { }
};
struct Comp {
    CVectorCompare_BestForTargeting VectorCompare;
    bool operator()(const PosEntry& a, const PosEntry& b) const
    {
        const float aValue = (VectorCompare.GetCompareValueForPoint(a.first));
        const float bValue = (VectorCompare.GetCompareValueForPoint(b.first));
        return aValue > bValue;
    }
};
static void Swap(PosEntry& a, PosEntry& b) { PosEntry t = a; a = b; b = t; }
static PosEntry* UnguardedPartition(PosEntry* first, PosEntry* last, PosEntry pivot, Comp comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Swap(*first, *last);
        ++first;
    }
}
int main()
{
    PosEntry v[9];
    const float xs[9] = { 3, 9, 1, 7, 5, 8, 2, 6, 4 };
    for (int i = 0; i < 9; ++i) { v[i].first = C3DVector(xs[i], 0.0f, 1.0f); v[i].second = 100 + i; }
    Comp comp; comp.VectorCompare.AnchorPos = C3DVector(0.0f, 0.0f, 0.0f);
    PosEntry pivot = v[4];   // value 5 (median of 1..9): guards exist on both sides
    PosEntry* cut = UnguardedPartition(v, v + 9, pivot, comp);
    int k = (int)(cut - v);
    bool ok = k > 0 && k < 9;
    for (int i = 0; i < k; ++i) if (comp.VectorCompare.GetCompareValueForPoint(v[i].first) < 5.0f) ok = false;
    for (int i = k; i < 9; ++i) if (comp.VectorCompare.GetCompareValueForPoint(v[i].first) > 5.0f) ok = false;
    // the pairs move whole: X and the id stay together
    for (int i = 0; i < 9; ++i) if ((long)xs[v[i].second - 100] != (long)v[i].first.X) ok = false;
    long sum = 0; for (int i = 0; i < 9; ++i) sum += v[i].second;
    if (sum != 9 * 100 + 36) ok = false;
    if (!ok) { std::printf("00807250_TEST FAIL\n"); return 1; }
    std::printf("00807250_TEST PASS\n");
    return 0;
}