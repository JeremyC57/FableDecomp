#include <cstdio>
#include <cstring>
struct Hero { bool dead; };
static int g_saves = 0; static bool g_lastQuest = false; static bool g_locked = false; static bool g_lockedDuring = false;
static void SaveGameState(bool q) { ++g_saves; g_lastQuest = q; g_lockedDuring = g_locked; }
static bool AutoSave(Hero* h, bool questScreens)
{
    if (h && !h->dead && !g_locked) { g_locked = true; if (questScreens) SaveGameState(true); else SaveGameState(false); g_locked = false; return true; }
    return false;
}
static bool Run()
{
    Hero h; h.dead = false;
    if (AutoSave(0, false)) return false;
    if (!AutoSave(&h, true) || !g_lastQuest || !g_lockedDuring || g_locked) return false;
    h.dead = true; if (AutoSave(&h, false)) return false;
    h.dead = false; g_locked = true; if (AutoSave(&h, false)) return false;
    return g_saves == 1;
}
int main()
{
    if (!Run()) { std::printf("004a0850_TEST FAIL\n"); return 1; }
    std::printf("004a0850_TEST PASS\n");
    return 0;
}
