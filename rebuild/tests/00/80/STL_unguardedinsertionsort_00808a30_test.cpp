// Behaviour test for 0x00808A30 std::__unguarded_insertion_sort front end: forwards (first, last, (T*)0, comp).
#include <cstdio>

struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } C3DVector() : X(0), Y(0), Z(0) { } };
struct PairVectorLong { C3DVector first; long second; PairVectorLong() : second(0) { } PairVectorLong(const PairVectorLong& o) : first(o.first), second(o.second) { } };
struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos; C3DVector PreferredLine;
    float AngleCompare, SqrDistCompare, OverrideShortDistanceFactor, OverrideShortDistanceFalloff;
    bool CompareInXYPlane;
};
struct CCompareTargetedPositionsForFirstPersonTargeting { CVectorCompare_BestForTargeting VectorCompare; };
typedef CCompareTargetedPositionsForFirstPersonTargeting CComp;

static PairVectorLong* g_first; static PairVectorLong* g_last; static PairVectorLong* g_type; static CComp g_comp; static int g_calls = 0;
static void __unguarded_insertion_sort_aux(PairVectorLong* first, PairVectorLong* last, PairVectorLong* valueType, CComp comp)
{
    ++g_calls; g_first = first; g_last = last; g_type = valueType; g_comp = comp;
}
static void __unguarded_insertion_sort(PairVectorLong* first, PairVectorLong* last, CComp comp)
{
    __unguarded_insertion_sort_aux(first, last, (PairVectorLong*)0, comp);
}

int main()
{
    PairVectorLong a[3];
    CComp comp;
    comp.VectorCompare.AnchorPos.X = 1.5f; comp.VectorCompare.AnchorPos.Y = 0.0f; comp.VectorCompare.AnchorPos.Z = 0.0f;
    comp.VectorCompare.PreferredLine.X = 0.0f; comp.VectorCompare.PreferredLine.Y = 0.0f; comp.VectorCompare.PreferredLine.Z = -2.0f;
    comp.VectorCompare.AngleCompare = 0.25f; comp.VectorCompare.SqrDistCompare = 4.0f;
    comp.VectorCompare.OverrideShortDistanceFactor = 0.5f; comp.VectorCompare.OverrideShortDistanceFalloff = 8.0f;
    comp.VectorCompare.CompareInXYPlane = true;
    g_type = a;
    __unguarded_insertion_sort(a, a + 3, comp);
    if (g_calls != 1 || g_first != a || g_last != a + 3 || g_type != 0) { std::printf("00808a30_TEST FAIL\n"); return 1; }
    if (g_comp.VectorCompare.AnchorPos.X != 1.5f || g_comp.VectorCompare.PreferredLine.Z != -2.0f || g_comp.VectorCompare.SqrDistCompare != 4.0f
        || g_comp.VectorCompare.OverrideShortDistanceFalloff != 8.0f || !g_comp.VectorCompare.CompareInXYPlane) { std::printf("00808a30_TEST FAIL\n"); return 1; }
    std::printf("00808a30_TEST PASS\n");
    return 0;
}