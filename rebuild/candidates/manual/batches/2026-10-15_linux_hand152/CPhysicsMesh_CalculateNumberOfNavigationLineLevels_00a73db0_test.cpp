#include <cstdio>
#include <cstring>
static const char* g_names[4]; static int g_nnames = 0;
static bool Has(const char* n) { for (int i = 0; i < g_nnames; ++i) if (std::strcmp(g_names[i], n) == 0) return true; return false; }
static long Levels()
{
    long levels = 0;
    for (long i = 0; i < 9; ++i) {
        char name[32]; std::sprintf(name, "NAV_LAYER_0%ld", i + 1);
        if (Has(name)) levels = i + 1;
    }
    return levels;
}
int main()
{
    int ok = Levels() == 0;
    g_names[g_nnames++] = "NAV_LAYER_01"; g_names[g_nnames++] = "NAV_LAYER_03";
    ok = ok && Levels() == 3;
    g_names[g_nnames++] = "NAV_LAYER_010";
    ok = ok && Levels() == 3;
    if (!ok) { std::printf("00a73db0_TEST FAIL\n"); return 1; }
    std::printf("00a73db0_TEST PASS\n");
    return 0;
}
