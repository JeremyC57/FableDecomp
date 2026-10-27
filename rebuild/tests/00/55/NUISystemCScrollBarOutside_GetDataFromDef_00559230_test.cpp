// behaviour test for 0x00559230: only the configured action types are added, each with its type
#include <cstdio>
static int g_added[2]; static int g_n = 0;
struct Def { long a, b; };
static void Add(long type) { g_added[g_n++] = (int)type; }
static void GetDataFromDef(const Def& d) { if (d.a) Add(d.a); if (d.b) Add(d.b); }
int main()
{
    Def d1 = { 3, 0 }; GetDataFromDef(d1); Def d2 = { 0, 0 }; GetDataFromDef(d2); Def d3 = { 0, 7 }; GetDataFromDef(d3);
    if (g_n != 2 || g_added[0] != 3 || g_added[1] != 7) { std::printf("00559230_TEST FAIL\n"); return 1; }
    std::printf("00559230_TEST PASS\n");
    return 0;
}