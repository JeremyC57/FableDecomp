// Behaviour test for 0x0080C020 sort<CTargetPoint, CThingCompare_BestForTargeting2> (model copy): empty ranges do
// nothing; otherwise the introsort loop gets depth 2*floor(log2(n)) and a comparator copy, then the final insertion sort.
#include <cstdio>
struct CTargetPoint { int words[7]; };
struct Comp { int Hero; int words[11]; float EnemyPreferenceFactor; };
static int g_calls = 0, g_depth = -1, g_final = 0; static float g_factor = 0;
static CTargetPoint* g_first = 0; static CTargetPoint* g_last = 0;
static void introsort_loop_stub(CTargetPoint* first, CTargetPoint* last, CTargetPoint* tag, int depth, Comp comp)
{ ++g_calls; g_first = first; g_last = last; g_depth = tag == 0 ? depth : -2; g_factor = comp.EnemyPreferenceFactor; }
static void final_insertion_sort_stub(CTargetPoint* first, CTargetPoint* last, Comp comp)
{ if (first == g_first && last == g_last && comp.Hero == 77) ++g_final; }
static int lg_model(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
static void sort_model(CTargetPoint* first, CTargetPoint* last, Comp comp)
{
    if (first != last) {
        introsort_loop_stub(first, last, (CTargetPoint*)0, lg_model((int)(last - first)) * 2, comp);
        final_insertion_sort_stub(first, last, comp);
    }
}
int main()
{
    CTargetPoint v[40];
    Comp c; c.Hero = 77; c.EnemyPreferenceFactor = 1.5f;
    int ok = 1;
    sort_model(v, v, c);
    if (g_calls != 0 || g_final != 0) ok = 0;
    sort_model(v, v + 40, c);                        // lg(40) = 5 -> depth 10
    if (g_calls != 1 || g_depth != 10 || g_final != 1 || g_factor != 1.5f || g_first != v || g_last != v + 40) ok = 0;
    sort_model(v, v + 1, c);                         // lg(1) = 0 -> depth 0
    if (g_calls != 2 || g_depth != 0 || g_final != 2) ok = 0;
    sort_model(v + 3, v + 35, c);                    // lg(32) = 5 -> depth 10
    if (g_calls != 3 || g_depth != 10 || g_final != 3) ok = 0;
    if (!ok) { std::printf("0080c020_TEST FAIL\n"); return 1; }
    std::printf("0080c020_TEST PASS\n");
    return 0;
}