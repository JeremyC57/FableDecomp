#include <cstdio>

struct CMap { unsigned __int64 AllocateMapThingUID() { return 0x100000002ULL; } };
static CMap g_map; static long g_idx = -1;
struct V { virtual CMap* GetMap(long i) { g_idx = i; return &g_map; }
    unsigned __int64 Alloc(long i) { CMap* m = GetMap(i); return m->AllocateMapThingUID(); } };
int main()
{
    V w; unsigned __int64 r = w.Alloc(5);
    if (g_idx != 5 || r != 0x100000002ULL) { std::printf("004fb330_TEST FAIL\n"); return 1; }
    std::printf("004fb330_TEST PASS\n");
    return 0;
}