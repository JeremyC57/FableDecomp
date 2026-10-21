// Behaviour test for CTCInventoryTrade::ConstructSubtitlesPC (0x0059E4D7): each of the thirteen category entries is looked up
// by name in order and handed to FinalConstructSubtitlesPC with the same map and list.
#include <cstdio>
#include <cstring>

static const char* g_names[13] = {
    "PC_TRADING_MENU_MELEE_ENTRY", "PC_TRADING_MENU_RANGED_ENTRY", "PC_TRADING_MENU_HEAD_ENTRY",
    "PC_TRADING_MENU_TORSO_ENTRY", "PC_TRADING_MENU_HANDS_ENTRY", "PC_TRADING_MENU_LEGS_ENTRY",
    "PC_TRADING_MENU_FEET_ENTRY", "PC_TRADING_MENU_SUITS_ENTRY", "PC_TRADING_MENU_TROPHIES_ENTRY",
    "PC_TRADING_MENU_POTIONS_ENTRY", "PC_TRADING_MENU_GIFTS_ENTRY", "PC_TRADING_MENU_PRODUCE_ENTRY",
    "PC_TRADING_MENU_OTHERS_ENTRY" };
struct Menu {
    int found;
    int FindChild(const char* name) { for (int i = 0; i < 13; ++i) if (!std::strcmp(name, g_names[i])) return 100 + i; return -1; }
};
static int g_seen[13]; static int g_count = 0; static int g_mapOk = 1;
static void Final(int component, int* subtitles, int* list, int* expectMap, int* expectList)
{
    if (subtitles != expectMap || list != expectList) g_mapOk = 0;
    g_seen[g_count++] = component;
}
int main()
{
    Menu menu; int map = 0, list = 0;
    for (int i = 0; i < 13; ++i)
        Final(menu.FindChild(g_names[i]), &map, &list, &map, &list);
    if (g_count != 13 || !g_mapOk) { std::printf("0059e4d7_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 13; ++i)
        if (g_seen[i] != 100 + i) { std::printf("0059e4d7_TEST FAIL\n"); return 1; }
    std::printf("0059e4d7_TEST PASS\n");
    return 0;
}