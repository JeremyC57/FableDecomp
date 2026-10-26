// Behaviour test for 007fa290 (std::__unguarded_insertion_sort_aux over CThing* with CThingCompare_BestForTargeting2): the range comes out ordered by key, every element moved whole, copies balanced.
#include <cstdio>
#include <new>
class CThing {
public:
    float score;
};
static CThing g_things[8];
class C3DVector { public: float X, Y, Z; };
class CVectorCompare_BestForTargeting { public:
    C3DVector AnchorPos;
    C3DVector PreferredLine;
    float     AngleCompare;
    float     SqrDistCompare;
    float     OverrideShortDistanceFactor;
    float     OverrideShortDistanceFalloff;
    bool      CompareInXYPlane;
};
static int g_badComparator = 0;
class CThingCompare_BestForTargeting2 { public:
    CThing* Hero;
    CVectorCompare_BestForTargeting VectorCompare;
    float EnemyPreferenceFactor;
    bool operator()(const CThing* a, const CThing* b) const
    {
        if (EnemyPreferenceFactor != 2.5f || VectorCompare.AnchorPos.Y != 7.0f) ++g_badComparator;   // passed by value intact
        return a->score < b->score;
    }
};
static CThingCompare_BestForTargeting2 MakeCompare()
{
    CThingCompare_BestForTargeting2 c;
    c.Hero = 0;
    c.VectorCompare.AnchorPos.X = 1.0f; c.VectorCompare.AnchorPos.Y = 7.0f; c.VectorCompare.AnchorPos.Z = 0.0f;
    c.VectorCompare.PreferredLine = c.VectorCompare.AnchorPos;
    c.VectorCompare.AngleCompare = c.VectorCompare.SqrDistCompare = 0.0f;
    c.VectorCompare.OverrideShortDistanceFactor = c.VectorCompare.OverrideShortDistanceFalloff = 0.0f;
    c.VectorCompare.CompareInXYPlane = true;
    c.EnemyPreferenceFactor = 2.5f;
    return c;
}
typedef CThing* T;
typedef CThingCompare_BestForTargeting2 C;
static int KeyOf(const T& x) { return (int)x->score; }
static bool Consistent(const T& x) { return x == &g_things[(int)x->score]; }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for 0x007FA0E0)
{
    T* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
void __unguarded_insertion_sort_aux(T* first, T* last, T*, C comp)
{
    for (T* i = first; i != last; ++i)
        __unguarded_linear_insert(i, *i, comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
int main()
{
    static const int keys[8] = { 0, 6, 3, 4, 1, 7, 2, 5 };
    for (int k = 0; k < 8; ++k) g_things[k].score = (float)k;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(&g_things[keys[i]]);
    C comp = MakeCompare();
    __unguarded_insertion_sort_aux(items + 1, items + 1, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) { std::printf("007fa290_TEST FAIL\n"); return 1; }
    __unguarded_insertion_sort_aux(items + 1, items + 8, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) { std::printf("007fa290_TEST FAIL\n"); return 1; }
    if (!(g_badComparator == 0)) { std::printf("007fa290_TEST FAIL\n"); return 1; }
    std::printf("007fa290_TEST PASS\n");
    return 0;
}