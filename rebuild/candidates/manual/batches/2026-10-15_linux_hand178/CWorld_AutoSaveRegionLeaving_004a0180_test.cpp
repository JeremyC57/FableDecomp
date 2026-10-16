#include <cstdio>
#include <cstring>
static int g_copies = 0; static char g_hero[32];
static bool Exists(const char* p, bool qsExists) { return qsExists && std::strcmp(p, "AutoSave.qs") == 0; }
static bool SaveHero(const char* n) { std::strcpy(g_hero, n); return true; }
static bool AutoSaveRegionLeaving(bool qsExists)
{
    if (Exists("AutoSave.qs", qsExists)) { ++g_copies; return SaveHero("AutoSave.rl"); }
    return false;
}
static bool Run() { if (AutoSaveRegionLeaving(false) || g_copies) return false; return AutoSaveRegionLeaving(true) && g_copies == 1 && std::strcmp(g_hero, "AutoSave.rl") == 0; }
int main()
{
    if (!Run()) { std::printf("004a0180_TEST FAIL\n"); return 1; }
    std::printf("004a0180_TEST PASS\n");
    return 0;
}
