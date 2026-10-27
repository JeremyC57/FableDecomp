// behaviour test for 0x004085A0 CUserProfileManager::GetMaxMeshDetail (model: cap, def value, no def; the def reference is released)
#include <cstdio>
static int g_cap = -1; static int g_refs = 0; static bool g_haveDef = true;
struct Def { float v; };
static Def g_def = { 0.75f };
static float Get()
{
    if (g_cap != -1) return (float)g_cap;
    Def* d = g_haveDef ? &g_def : 0;
    if (d) { ++g_refs; float v = d->v; --g_refs; return v; }
    return 0.0f;
}
int main()
{
    int ok = Get() == 0.75f && g_refs == 0;
    g_haveDef = false; ok = ok && Get() == 0.0f;
    g_cap = 2; ok = ok && Get() == 2.0f;
    if (!ok) { std::printf("004085a0_TEST FAIL\n"); return 1; }
    std::printf("004085a0_TEST PASS\n");
    return 0;
}
