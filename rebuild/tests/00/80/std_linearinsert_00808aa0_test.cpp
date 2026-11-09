// Behaviour test for 0x00808AA0 std::__linear_insert<pair<C3DVector,long>*, pair<C3DVector,long>,
// CCompareTargetedPositionsForFirstPersonTargeting> (C++03). The model is the row's: comp(a, b) = value(a.first) > value(b.first)
// (best score first); if val beats *first, [first, last) moves up one slot (copy_backward) and val goes to *first, otherwise
// val is inserted from the back (__unguarded_linear_insert: shift entries val beats, stop at the first one it does not).
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;
    // test stand-in for 0x007F9660's score
    float GetCompareValueForPoint(const C3DVector& p) const { return (p.X - AnchorPos.X) + 0.5f * (p.Y - AnchorPos.Y); }
    bool operator()(const C3DVector& a, const C3DVector& b) const
    {
        float aValue = (GetCompareValueForPoint(a));
        float bValue = (GetCompareValueForPoint(b));
        return aValue > bValue;
    }
};
struct PosEntry {
    C3DVector first;
    long second;
    PosEntry() { }
    PosEntry(const PosEntry& o) : first(o.first), second(o.second) { }
};
struct Comp {
    CVectorCompare_BestForTargeting VectorCompare;
    bool operator()(const PosEntry& a, const PosEntry& b) const { return VectorCompare(a.first, b.first); }
};
static PosEntry* CopyBackward(PosEntry* first, PosEntry* last, PosEntry* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void UnguardedLinearInsert(PosEntry* last, PosEntry val, Comp comp)
{
    PosEntry* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(PosEntry* first, PosEntry* last, PosEntry val, Comp comp)
{
    if (comp(val, *first)) {
        CopyBackward(first, last, last + 1);
        *first = val;
    } else
        UnguardedLinearInsert(last, val, comp);
}
static bool Check(const PosEntry* v, int n, const long* ids)
{
    for (int i = 0; i < n; ++i) if (v[i].second != ids[i] || (long)v[i].first.X != ids[i] - 100) return false;
    return true;
}
int main()
{
    Comp comp; comp.VectorCompare.AnchorPos = C3DVector(0.0f, 0.0f, 0.0f);
    bool ok = true;
    // sorted prefix (best first): 9 7 5 3; insert 8 -> 9 8 7 5 3 (from the back)
    PosEntry v[6];
    const long a[4] = { 109, 107, 105, 103 };
    for (int i = 0; i < 4; ++i) { v[i].first = C3DVector((float)(a[i] - 100), 0.0f, 1.0f); v[i].second = a[i]; }
    PosEntry val; val.first = C3DVector(8.0f, 0.0f, 1.0f); val.second = 108;
    LinearInsert(v, v + 4, val, comp);
    const long e1[5] = { 109, 108, 107, 105, 103 };
    if (!Check(v, 5, e1)) ok = false;
    // insert 12, better than the first -> shifted, placed first
    val.first = C3DVector(12.0f, 0.0f, 1.0f); val.second = 112;
    LinearInsert(v, v + 5, val, comp);
    const long e2[6] = { 112, 109, 108, 107, 105, 103 };
    if (!Check(v, 6, e2)) ok = false;
    // insert 1, worse than all: lands last
    PosEntry w[3];
    w[0].first = C3DVector(4.0f, 0.0f, 0.0f); w[0].second = 104;
    val.first = C3DVector(1.0f, 0.0f, 0.0f); val.second = 101;
    LinearInsert(w, w + 1, val, comp);
    const long e3[2] = { 104, 101 };
    if (!Check(w, 2, e3)) ok = false;
    if (!ok) { std::printf("00808aa0_TEST FAIL\n"); return 1; }
    std::printf("00808aa0_TEST PASS\n");
    return 0;
}