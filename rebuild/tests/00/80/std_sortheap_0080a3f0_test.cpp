// behaviour test for 0080a3f0: SGI sort_heap over (C3DVector, long) pairs with the 44-byte first-person targeting comparator passed by value
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy
};

struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;
    C3DVector PreferredLine;
    float     AngleCompare;
    float     SqrDistCompare;
    float     OverrideShortDistanceFactor;
    float     OverrideShortDistanceFalloff;
    bool      CompareInXYPlane;
    CVectorCompare_BestForTargeting() : AngleCompare(0), SqrDistCompare(0), OverrideShortDistanceFactor(0),
        OverrideShortDistanceFalloff(0), CompareInXYPlane(false) { }
};
struct SVectorLongPair {
    C3DVector first;
    long      second;
    SVectorLongPair() : second(0) { }
};
struct CCompareTargetedPositionsForFirstPersonTargeting {
    CVectorCompare_BestForTargeting VectorCompare;
    // squared distance from the anchor: nearer sorts first
    bool operator()(const SVectorLongPair& a, const SVectorLongPair& b) const
    {
        float ax = a.first.X - VectorCompare.AnchorPos.X, ay = a.first.Y - VectorCompare.AnchorPos.Y;
        float bx = b.first.X - VectorCompare.AnchorPos.X, by = b.first.Y - VectorCompare.AnchorPos.Y;
        return ax * ax + ay * ay < bx * bx + by * by;
    }
};
typedef SVectorLongPair ElemT;
typedef CCompareTargetedPositionsForFirstPersonTargeting CompT;
static CompT MakeComp()
{
    CompT comp;
    comp.VectorCompare.AnchorPos = C3DVector(-5.0f, 7.0f, 100.0f);
    comp.VectorCompare.CompareInXYPlane = true;
    return comp;
}
static void Fill(ElemT& e, int k)
{
    e.first = C3DVector(-5.0f + float(k), 7.0f, float(k) * 0.5f);
    e.second = k;
}
static int KeyOf(const ElemT& e) { return int(e.second); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    return e.first.X == -5.0f + float(k) && e.first.Y == 7.0f && e.first.Z == float(k) * 0.5f;
}
static bool Live(int) { return true; }
static bool Final() { return true; }

static void PushHeapT(ElemT* first, long hole, long top, ElemT value, CompT comp)
{
    long parent = (hole - 1) / 2;
    while (top < hole && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeapT(ElemT* first, long hole, long len, ElemT value, CompT comp)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < len; idx = 2 * idx + 2) {
        if (comp(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx];
        hole = idx;
    }
    if (idx == len) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeapT(first, hole, top, value, comp);
}

// the out-of-line callee (__pop_heap_aux in retail), modelled with the standard hole algorithm
static void __pop_heap_aux(ElemT* first, ElemT* last, ElemT*, CompT comp)
{
    ElemT value(*(last - 1));
    *(last - 1) = *first;
    AdjustHeapT(first, 0, long(last - 1 - first), value, comp);
}
// the reconstructed body: SGI sort_heap with pop_heap inline
inline void pop_heap(ElemT* first, ElemT* last, CompT comp)
{
    __pop_heap_aux(first, last, (ElemT*)0, comp);
}
static void sort_heap(ElemT* first, ElemT* last, CompT comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}
#define SORT(a, n, comp) sort_heap(a, a + n, comp)

int main()
{
    static const int keys[9] = { 5, 31, 12, 7, 64, 0, 99, 40, 2 };
    const int n = 9;
    int ok = 1;
    {
        ElemT a[9];
        CompT comp = MakeComp();
        int i = 0;
        for (i = 0; i < n; ++i) Fill(a[i], keys[i]);
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        for (i = 0; i < n; ++i) if (!Check(a[i])) ok = 0;
        if (KeyOf(a[0]) != 0 || KeyOf(a[n - 1]) != 99) ok = 0;
        if (!Live(n)) ok = 0;
        // re-heap the sorted output and sort again; a one-element range is left as it is
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        SORT(a, 1, comp);
        if (KeyOf(a[0]) != 0 || !Check(a[0])) ok = 0;
        if (!Live(n)) ok = 0;
    }
    if (!Final()) ok = 0;
    if (!ok) { std::printf("0080a3f0_TEST FAIL\n"); return 1; }
    std::printf("0080a3f0_TEST PASS\n");
    return 0;
}