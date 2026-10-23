// Behaviour test for CTCInventoryTrade::GetMenuBranchFromDefIndexPC @ 0x0059c0c6
#include <cstdio>
#include <cstring>
static const char* g_names[] = { "INVENTORY_CATEGORY_FOOD", "INVENTORY_CATEGORY_MELEE_WEAPONS", "INVENTORY_CATEGORY_RANGED_WEAPONS",
    "INVENTORY_CATEGORY_CLOTHES_HEADWEAR", "INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR", "INVENTORY_CATEGORY_CLOTHES_GLOVES",
    "INVENTORY_CATEGORY_CLOTHES_LEGWEAR", "INVENTORY_CATEGORY_CLOTHES_FOOTWEAR", "INVENTORY_CATEGORY_CLOTHES_SUITS",
    "INVENTORY_CATEGORY_GIFTS", "INVENTORY_CATEGORY_TROPHIES", "INVENTORY_CATEGORY_POTIONS" };
static long LookupDef(const char* name)
{
    for (int i = 0; i < 12; ++i)
        if (std::strcmp(g_names[i], name) == 0) return 100 + i;
    return -1;
}
static long GetInventoryCategory(long defIndex) { return defIndex / 10; }
static long GetInventoryTabState(long defIndex) { return defIndex % 10; }
struct CMenuTree {
    char last[64];
    virtual const char* GetBranch(const char* name) { std::strcpy(last, name); return last; }
};
static const char* Pick(long defIndex, CMenuTree* menu, long* type)
{
    LookupDef("INVENTORY_CATEGORY_FOOD");
    long melee = LookupDef("INVENTORY_CATEGORY_MELEE_WEAPONS");
    long ranged = LookupDef("INVENTORY_CATEGORY_RANGED_WEAPONS");
    long headwear = LookupDef("INVENTORY_CATEGORY_CLOTHES_HEADWEAR");
    long torso = LookupDef("INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR");
    long gloves = LookupDef("INVENTORY_CATEGORY_CLOTHES_GLOVES");
    long legwear = LookupDef("INVENTORY_CATEGORY_CLOTHES_LEGWEAR");
    long footwear = LookupDef("INVENTORY_CATEGORY_CLOTHES_FOOTWEAR");
    long suits = LookupDef("INVENTORY_CATEGORY_CLOTHES_SUITS");
    const char* branch;
    long category = GetInventoryCategory(defIndex);
    if (category == melee) { branch = menu->GetBranch("PC_TRADING_MENU_MELEE_ENTRY"); *type = 1; }
    else if (category == ranged) { branch = menu->GetBranch("PC_TRADING_MENU_RANGED_ENTRY"); *type = 1; }
    else if (category == headwear) { branch = menu->GetBranch("PC_TRADING_MENU_HEAD_ENTRY"); *type = 0; }
    else if (category == torso) { branch = menu->GetBranch("PC_TRADING_MENU_TORSO_ENTRY"); *type = 0; }
    else if (category == gloves) { branch = menu->GetBranch("PC_TRADING_MENU_HANDS_ENTRY"); *type = 0; }
    else if (category == legwear) { branch = menu->GetBranch("PC_TRADING_MENU_LEGS_ENTRY"); *type = 0; }
    else if (category == footwear) { branch = menu->GetBranch("PC_TRADING_MENU_FEET_ENTRY"); *type = 0; }
    else if (category == suits) { branch = menu->GetBranch("PC_TRADING_MENU_SUITS_ENTRY"); *type = 0; }
    else if (GetInventoryTabState(defIndex) == 0) { branch = menu->GetBranch("PC_TRADING_MENU_TROPHIES_ENTRY"); *type = 2; }
    else if (GetInventoryTabState(defIndex) == 1) { branch = menu->GetBranch("PC_TRADING_MENU_POTIONS_ENTRY"); *type = 2; }
    else if (GetInventoryTabState(defIndex) == 2) { branch = menu->GetBranch("PC_TRADING_MENU_GIFTS_ENTRY"); *type = 2; }
    else if (GetInventoryTabState(defIndex) == 3) { branch = menu->GetBranch("PC_TRADING_MENU_PRODUCE_ENTRY"); *type = 2; }
    else { branch = menu->GetBranch("PC_TRADING_MENU_OTHERS_ENTRY"); *type = 2; }
    return branch;
}
int main()
{
    CMenuTree menu;
    long type = -5;
    int ok = 1;
    if (std::strcmp(Pick(1010, &menu, &type), "PC_TRADING_MENU_MELEE_ENTRY") != 0 || type != 1) ok = 0;
    if (std::strcmp(Pick(1050, &menu, &type), "PC_TRADING_MENU_HANDS_ENTRY") != 0 || type != 0) ok = 0;
    if (std::strcmp(Pick(1001, &menu, &type), "PC_TRADING_MENU_POTIONS_ENTRY") != 0 || type != 2) ok = 0;
    if (std::strcmp(Pick(1009, &menu, &type), "PC_TRADING_MENU_OTHERS_ENTRY") != 0 || type != 2) ok = 0;
    if (!ok) { std::printf("0059c0c6_TEST FAIL\n"); return 1; }
    std::printf("0059c0c6_TEST PASS\n");
    return 0;
}
