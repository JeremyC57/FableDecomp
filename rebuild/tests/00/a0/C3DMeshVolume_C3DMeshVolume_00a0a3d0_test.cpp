#include <cstdio>
static int g_s = 0;
struct Str { char* p; Str() : p(0) { ++g_s; } };
struct CP { void* a; void* b; CP() : a(0), b(0) {} };
struct V { Str name; CP vol; V() : name(), vol() {} };
int main()
{
    V v;
    if (g_s != 1 || v.name.p || v.vol.a || v.vol.b || sizeof(V) != 12) { std::printf("00a0a3d0_TEST FAIL\n"); return 1; }
    std::printf("00a0a3d0_TEST PASS\n");
    return 0;
}