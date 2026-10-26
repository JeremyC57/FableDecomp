// behaviour test for 007fa980: SGI sort_heap over CThing* with the 52-byte CThingCompare_BestForTargeting2 comparator passed by value
#include <cstdio>
#include <cstring>
#include <cstdlib>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy
};

class CThing {
public:
    C3DVector Position;
    long      m_id;
};
class CVectorCompare_BestForTargeting { public:
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
class CThingCompare_BestForTargeting2 { public:
    CThing* Hero;
    CVectorCompare_BestForTargeting VectorCompare;
    float EnemyPreferenceFactor;
    CThingCompare_BestForTargeting2() : Hero(0), EnemyPreferenceFactor(0) { }
    // nearer to the anchor (scaled by the enemy preference) is better; Hero must survive every by-value copy
    bool operator()(CThing* a, CThing* b) const
    {
        float da = (a->Position.X - VectorCompare.AnchorPos.X) * EnemyPreferenceFactor;
        float db = (b->Position.X - VectorCompare.AnchorPos.X) * EnemyPreferenceFactor;
        return da + float(Hero->m_id) < db + float(Hero->m_id);
    }
};
static CThing g_things[100];
static CThing g_hero;
typedef CThing* ElemT;
typedef CThingCompare_BestForTargeting2 CompT;
static CompT MakeComp()
{
    CompT comp;
    g_hero.m_id = 3;
    comp.Hero = &g_hero;
    comp.VectorCompare.AnchorPos = C3DVector(10.0f, 0.0f, 0.0f);
    comp.VectorCompare.CompareInXYPlane = true;
    comp.EnemyPreferenceFactor = 2.0f;
    return comp;
}
static void Fill(ElemT& e, int k)
{
    g_things[k].Position = C3DVector(float(k) + 10.0f, 0.0f, 0.0f);
    g_things[k].m_id = k;
    e = &g_things[k];
}
static int KeyOf(const ElemT& e) { return int(e->m_id); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    return e == &g_things[k];
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
    if (!ok) { std::printf("007fa980_TEST FAIL\n"); return 1; }
    std::printf("007fa980_TEST PASS\n");
    return 0;
}