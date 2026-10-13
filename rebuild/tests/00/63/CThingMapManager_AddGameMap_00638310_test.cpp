#include <cstdio>

struct Rec { unsigned char raw[72]; };
struct Tab { unsigned char pad[0x20]; Rec* Maps; };
static Tab g_tab; static Rec g_recs[4]; static long g_i = -1; static Rec* g_r = 0;
struct W { virtual Tab* GetGameMapTable() { return &g_tab; } };
struct M { void* vt; W* PWorld; void AddThingMap(long i, Rec* r) { g_i = i; g_r = r; }
    void AddGameMap(long i) { Tab* t = PWorld->GetGameMapTable(); AddThingMap(i, &t->Maps[i]); } };
int main()
{
    g_tab.Maps = g_recs; W w; M m; m.PWorld = &w; m.AddGameMap(3);
    if (g_i != 3 || g_r != &g_recs[3]) { std::printf("00638310_TEST FAIL\n"); return 1; }
    std::printf("00638310_TEST PASS\n");
    return 0;
}