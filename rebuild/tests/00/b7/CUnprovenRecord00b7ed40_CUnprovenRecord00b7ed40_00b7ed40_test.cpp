#include <cstdio>
#include <cstdlib>
// Behaviour model: the destructor frees exactly the allocated blocks, later member first.
static int g_order[2], g_n;
struct V { void* start; int tag; ~V() { if (start) { g_order[g_n++] = tag; std::free(start); } } };
struct R { unsigned long w; V a; V b; };
int main()
{
    {
        R r; r.w = 0; r.a.start = std::malloc(4); r.a.tag = 1; r.b.start = std::malloc(4); r.b.tag = 2;
    }
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("00b7ed40_TEST FAIL\n"); return 1; }
    {
        R r; r.w = 0; r.a.start = 0; r.a.tag = 3; r.b.start = 0; r.b.tag = 4;
    }
    if (g_n != 2) { std::printf("00b7ed40_TEST FAIL\n"); return 1; }
    std::printf("00b7ed40_TEST PASS\n");
    return 0;
}