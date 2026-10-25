// Behaviour test for 0x00809590 std::__insertion_sort over the first-person targeting (position, index) pairs with a 0x2C-byte comparator by value.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct PairVectorLong {
    C3DVector first; long second;
    PairVectorLong() : second(0) { }
    PairVectorLong(const PairVectorLong& o) : first(o.first), second(o.second) { }
};
struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos; C3DVector PreferredLine;
    float AngleCompare, SqrDistCompare, OverrideShortDistanceFactor, OverrideShortDistanceFalloff;
    bool CompareInXYPlane;
    float Value(const C3DVector& p) const   // larger is better: closer to the anchor
    {
        float dx = p.X - AnchorPos.X, dy = p.Y - AnchorPos.Y, dz = CompareInXYPlane ? 0.0f : p.Z - AnchorPos.Z;
        return -(dx * dx + dy * dy + dz * dz) * SqrDistCompare;
    }
};
struct CCompareTargetedPositionsForFirstPersonTargeting {
    CVectorCompare_BestForTargeting VectorCompare;
    bool operator()(const PairVectorLong& a, const PairVectorLong& b) const { return VectorCompare.Value(a.first) > VectorCompare.Value(b.first); }
};
typedef CCompareTargetedPositionsForFirstPersonTargeting CComp;

static int g_calls = 0;
static void __linear_insert(PairVectorLong* first, PairVectorLong* last, PairVectorLong value, CComp comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (PairVectorLong* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        PairVectorLong* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(PairVectorLong* first, PairVectorLong* last, CComp comp)
{
    if (first == last)
        return;
    for (PairVectorLong* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    PairVectorLong a[4];
    float xs[4] = { 5.0f, 1.0f, 3.0f, 0.5f };
    for (int k = 0; k < 4; ++k) { a[k].first = C3DVector(xs[k], 0.0f, 100.0f * k); a[k].second = k; }
    CComp comp;
    comp.VectorCompare.AnchorPos = C3DVector(0, 0, 0); comp.VectorCompare.PreferredLine = C3DVector(1, 0, 0);
    comp.VectorCompare.AngleCompare = 1.0f; comp.VectorCompare.SqrDistCompare = 1.0f;
    comp.VectorCompare.OverrideShortDistanceFactor = 0.0f; comp.VectorCompare.OverrideShortDistanceFalloff = 0.0f;
    comp.VectorCompare.CompareInXYPlane = true;   // Z ignored
    __insertion_sort(a, a + 4, comp);
    long order[4] = { 3, 1, 2, 0 };
    for (int k = 0; k < 4; ++k)
        if (a[k].second != order[k]) { std::printf("00809590_TEST FAIL\n"); return 1; }
    if (g_calls != 3) { std::printf("00809590_TEST FAIL\n"); return 1; }
    std::printf("00809590_TEST PASS\n");
    return 0;
}