// Behaviour test for 0x004FA860: two-layer poly map operator=: copies settings and both layers, keeps the 5-byte header, resets the trailing cache
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Row { unsigned int bits[32]; unsigned int count; };
struct Layer { Row rows[256]; };
struct Map {
    long refCount; unsigned char type; long s05, s09, s0d, s11; unsigned char f15; Layer a; long s8416, s841a; Layer b; void* cache;
    Map& operator=(const Map& o)
    {
        s05 = o.s05; s09 = o.s09; s0d = o.s0d; s11 = o.s11; f15 = o.f15; a = o.a; s8416 = o.s8416; s841a = o.s841a; b = o.b; cache = 0;
        return *this;
    }
};
static Map g_x, g_y;
static void Run()
{
    g_x.refCount = 3; g_x.type = 8; g_x.s05 = 1; g_x.s09 = 2; g_x.s0d = 3; g_x.s11 = 4; g_x.f15 = 5; g_x.s8416 = 6; g_x.s841a = 7;
    g_x.a.rows[10].bits[3] = 0xABCD; g_x.b.rows[255].count = 9; g_x.cache = &g_x;
    g_y.refCount = 1; g_y.type = 2; g_y.cache = &g_y;
    Map& r = (g_y = g_x);
    CHECK(&r == &g_y && g_y.refCount == 1 && g_y.type == 2);
    CHECK(g_y.s05 == 1 && g_y.s11 == 4 && g_y.f15 == 5 && g_y.s8416 == 6 && g_y.s841a == 7);
    CHECK(g_y.a.rows[10].bits[3] == 0xABCD && g_y.b.rows[255].count == 9 && g_y.cache == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("004fa860_TEST FAIL\n"); return 1; }
    std::printf("004fa860_TEST PASS\n");
    return 0;
}
