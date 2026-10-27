// Behaviour test for std::__unguarded_linear_insert<list<CAnimSet>::const_iterator*> (0x0070FCB0): shifts higher-priority iterators up and drops val in the hole.
#include <cstdio>
class CAnimSet { public: int Priority; };
struct It { CAnimSet* set; };
static void unguarded_linear_insert(It* last, It val)
{
    It* next = last; --next;
    while (val.set->Priority < next->set->Priority) { *last = *next; last = next; --next; }
    *last = val;
}
int main()
{
    CAnimSet s[5]; int pr[5] = { 1, 3, 5, 7, 4 };
    It v[5];
    for (int i = 0; i < 5; ++i) { s[i].Priority = pr[i]; v[i].set = &s[i]; }
    unguarded_linear_insert(v + 4, v[4]);
    int want[5] = { 1, 3, 4, 5, 7 };
    for (int i = 0; i < 5; ++i) if (v[i].set->Priority != want[i]) { std::printf("0070fcb0_TEST FAIL\n"); return 1; }
    if (v[2].set != &s[4]) { std::printf("0070fcb0_TEST FAIL\n"); return 1; }
    std::printf("0070fcb0_TEST PASS\n");
    return 0;
}