// Behaviour test for CUserProfileManager::Load (0040d350): only an existing, signed, opened file is read and closed.
#include <cstdio>
static int g_loads = 0, g_closes = 0; static bool g_exists = true, g_signed = true, g_opens = true;
struct File { bool Opened; File() : Opened(g_opens) { } ~File() { Close(); } void Close() { ++g_closes; } };
struct Mgr {
    const char* name;
    void LoadGameState(File*) { ++g_loads; }
    void Load()
    {
        const char* path = name;
        if (path[0] != 0 && g_exists && g_signed) {
            File file;
            if (file.Opened) { LoadGameState(&file); file.Close(); }
        }
    }
};
int main()
{
    Mgr m; m.name = "profile";
    m.Load(); bool ok = g_loads == 1 && g_closes == 2;          // explicit Close plus the destructor's
    g_opens = false; m.Load(); ok = ok && g_loads == 1 && g_closes == 3;
    g_opens = true; g_signed = false; m.Load(); ok = ok && g_loads == 1 && g_closes == 3;
    g_signed = true; m.name = ""; m.Load(); ok = ok && g_loads == 1;
    if (!ok) { std::printf("0040d350_TEST FAIL\n"); return 1; }
    std::printf("0040d350_TEST PASS\n");
    return 0;
}