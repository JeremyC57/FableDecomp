// behaviour test for 0x004EDAF3 __copy_ptrs (model: n assignments forward, the end of the copy returned)
#include <cstdio>
static int g_assigns = 0;
struct E { int a, b; E& operator=(const E& o) { a = o.a; b = o.b; ++g_assigns; return *this; } };
int main()
{
    E src[3] = { { 1, 2 }, { 3, 4 }, { 5, 6 } }, dst[3] = { { 0, 0 }, { 0, 0 }, { 0, 0 } };
    E* r = dst; for (int n = 3; n > 0; --n) { *r = src[3 - n]; ++r; }
    int ok = r == dst + 3 && dst[2].b == 6 && g_assigns == 3;
    if (!ok) { std::printf("004edaf3_TEST FAIL\n"); return 1; }
    std::printf("004edaf3_TEST PASS\n");
    return 0;
}