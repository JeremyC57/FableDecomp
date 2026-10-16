#include <cstdio>
#include <cstring>
// Behaviour model of CTCInventoryAbilities::GetMenuEntryFromCategory (0x005EE288): categories 0/1/2 look up the
// attack / surround / physical spells entry, with the PC_ name in live-GUI mode and the UI_ name otherwise; others 0.
static const char* Name(int category, bool live)
{
    switch (category) {
    case 0: return live ? "PC_MENU_ENTRY_ATTACK_SPELLS" : "UI_MENU_ENTRY_ATTACK_SPELLS";
    case 1: return live ? "PC_MENU_ENTRY_SURROUND_SPELLS" : "UI_MENU_ENTRY_SURROUND_SPELLS";
    case 2: return live ? "PC_MENU_ENTRY_PHYSICAL_SPELLS" : "UI_MENU_ENTRY_PHYSICAL_SPELLS";
    }
    return 0;
}
int main()
{
    if (std::strcmp(Name(0, true), "PC_MENU_ENTRY_ATTACK_SPELLS") || std::strcmp(Name(1, false), "UI_MENU_ENTRY_SURROUND_SPELLS")
        || std::strcmp(Name(2, true), "PC_MENU_ENTRY_PHYSICAL_SPELLS") || Name(3, true) != 0 || Name(-1, false) != 0) { std::printf("005ee288_TEST FAIL\n"); return 1; }
    std::printf("005ee288_TEST PASS\n");
    return 0;
}