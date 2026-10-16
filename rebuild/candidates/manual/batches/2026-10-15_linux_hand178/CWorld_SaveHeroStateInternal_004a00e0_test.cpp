#include <cstdio>
#include <cstring>
static bool g_serialising = false; static bool g_sawFlag = false; static char g_saved[32];
static bool SaveState(const char* path, bool b) { g_sawFlag = g_serialising; std::strcpy(g_saved, path); return b; }
static bool SaveHero(const char* name, bool b, bool profileOk)
{
    char file[32]; std::strcpy(file, name); std::strcat(file, ".hs");
    char path[40]; path[0] = 0; if (profileOk) { std::strcpy(path, "dir/"); std::strcat(path, file); }
    if (path[0] != 0) { g_serialising = true; bool r = SaveState(path, b); g_serialising = false; return r; }
    return false;
}
static bool Run() { if (!SaveHero("AutoSave", true, true) || !g_sawFlag || g_serialising || std::strcmp(g_saved, "dir/AutoSave.hs")) return false; return !SaveHero("x", true, false); }
int main()
{
    if (!Run()) { std::printf("004a00e0_TEST FAIL\n"); return 1; }
    std::printf("004a00e0_TEST PASS\n");
    return 0;
}
