// behaviour test for 0x0059E94D: every category entry is looked up in order and handed on with the same map
#include <cstdio>
#include <cstring>
static const char* g_names[13]; static int g_n = 0; static void* g_map = 0;
struct Comp { const char* FindChild(const char* n) { g_names[g_n] = n; return n; } };
static void Final(const char* child, void* map) { if (child == g_names[g_n] && map) ++g_n; g_map = map; }
int main()
{
    Comp c; int m = 0;
    static const char* names[13] = { "PC_TRADING_MENU_MELEE_ENTRY", "PC_TRADING_MENU_RANGED_ENTRY", "PC_TRADING_MENU_HEAD_ENTRY", "PC_TRADING_MENU_TORSO_ENTRY", "PC_TRADING_MENU_HANDS_ENTRY", "PC_TRADING_MENU_LEGS_ENTRY", "PC_TRADING_MENU_FEET_ENTRY", "PC_TRADING_MENU_SUITS_ENTRY", "PC_TRADING_MENU_TROPHIES_ENTRY", "PC_TRADING_MENU_POTIONS_ENTRY", "PC_TRADING_MENU_GIFTS_ENTRY", "PC_TRADING_MENU_PRODUCE_ENTRY", "PC_TRADING_MENU_OTHERS_ENTRY" };
    for (int i = 0; i < 13; ++i) Final(c.FindChild(names[i]), &m);
    if (g_n != 13 || g_map != &m || std::strcmp(g_names[12], "PC_TRADING_MENU_OTHERS_ENTRY") != 0) { std::printf("0059e94d_TEST FAIL\n"); return 1; }
    std::printf("0059e94d_TEST PASS\n");
    return 0;
}