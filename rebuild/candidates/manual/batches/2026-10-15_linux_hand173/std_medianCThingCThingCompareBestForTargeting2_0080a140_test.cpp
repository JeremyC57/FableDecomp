// behaviour test for std::__median<CThing*, CThingCompare_BestForTargeting2> (0x0080A140)
#include <cstdio>
struct CThing { int v; };
struct CThingCompare_BestForTargeting2 { int calls; bool operator()(CThing* a, CThing* b) { ++calls; return a->v < b->v; } };
template <class _Tp, class _Compare> _Tp median3(_Tp __a, _Tp __b, _Tp __c, _Compare __comp)
{
    if (__comp(__a, __b)) { if (__comp(__b, __c)) return __b; else if (__comp(__a, __c)) return __c; else return __a; }
    else if (__comp(__a, __c)) return __a;
    else if (__comp(__b, __c)) return __c;
    else return __b;
}
int main()
{
    CThing t[3] = { {1}, {2}, {3} };
    int perm[6][3] = { {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0} };
    CThingCompare_BestForTargeting2 cmp; cmp.calls = 0;
    int bad = 0;
    for (int i = 0; i < 6; ++i) if (median3(&t[perm[i][0]], &t[perm[i][1]], &t[perm[i][2]], cmp)->v != 2) bad = 1;
    if (bad) { std::printf("0080a140_TEST FAIL\n"); return 1; }
    std::printf("0080a140_TEST PASS\n");
    return 0;
}
