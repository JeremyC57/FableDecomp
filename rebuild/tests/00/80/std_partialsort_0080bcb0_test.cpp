// Behaviour test for 0x0080BCB0 partial_sort<CTargetPoint, CThingCompare_BestForTargeting2> (model copy): the front
// end forwards first/middle/last, a null value-type tag and an intact comparator copy to __partial_sort.
#include <cstdio>
struct CTargetPoint { int words[7]; };
struct Comp { int Hero; float f[10]; bool CompareInXYPlane; float EnemyPreferenceFactor; };
static int g_ok = 0;
static CTargetPoint* g_expect[3];
static void partial_sort_core_stub(CTargetPoint* first, CTargetPoint* middle, CTargetPoint* last, CTargetPoint* tag, Comp comp)
{
    if (first == g_expect[0] && middle == g_expect[1] && last == g_expect[2] && tag == 0 &&
        comp.Hero == 5 && comp.f[9] == 2.0f && comp.CompareInXYPlane && comp.EnemyPreferenceFactor == 0.25f) ++g_ok;
}
static void partial_sort_model(CTargetPoint* first, CTargetPoint* middle, CTargetPoint* last, Comp comp)
{
    partial_sort_core_stub(first, middle, last, (CTargetPoint*)0, comp);
}
int main()
{
    CTargetPoint v[10];
    Comp c; c.Hero = 5; for (int i = 0; i < 10; ++i) c.f[i] = (float)i * 0.25f + 0.0f; c.f[9] = 2.0f;
    c.CompareInXYPlane = true; c.EnemyPreferenceFactor = 0.25f;
    g_expect[0] = v; g_expect[1] = v + 4; g_expect[2] = v + 10;
    partial_sort_model(v, v + 4, v + 10, c);
    g_expect[0] = v + 2; g_expect[1] = v + 9; g_expect[2] = v + 9;
    partial_sort_model(v + 2, v + 9, v + 9, c);
    if (g_ok != 2) { std::printf("0080bcb0_TEST FAIL\n"); return 1; }
    std::printf("0080bcb0_TEST PASS\n");
    return 0;
}