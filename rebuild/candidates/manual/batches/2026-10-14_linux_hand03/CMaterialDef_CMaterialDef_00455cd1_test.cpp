#include <cstdio>

struct Vec { void* a; void* b; void* c; Vec() : a(0), b(0), c(0) {} };
static int g_maps = 0; struct Map { Map() { ++g_maps; } };
struct Def { unsigned char base[0x3c]; long s1, s2; float r1, r2; bool b1, b2; Vec v1, v2, v3, v4, v5; Map m; Def() {} };
int main()
{
    Def d; if (d.v1.a || d.v3.b || d.v5.c || g_maps != 1) { std::printf("00455cd1_TEST FAIL\n"); return 1; }
    std::printf("00455cd1_TEST PASS\n");
    return 0;
}
