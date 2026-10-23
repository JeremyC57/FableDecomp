// Behaviour test for CTCInventoryTrade::GetTypeFromDefIndexPC @ 0x0059C447
#include <cstdio>
#include <cstring>
static const char* g_names[] = { "INVENTORY_CATEGORY_FOOD", "INVENTORY_CATEGORY_MELEE_WEAPONS", "INVENTORY_CATEGORY_RANGED_WEAPONS",
    "INVENTORY_CATEGORY_CLOTHES_HEADWEAR", "INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR", "INVENTORY_CATEGORY_CLOTHES_GLOVES",
    "INVENTORY_CATEGORY_CLOTHES_LEGWEAR", "INVENTORY_CATEGORY_CLOTHES_FOOTWEAR", "INVENTORY_CATEGORY_CLOTHES_SUITS",
    "INVENTORY_CATEGORY_GIFTS", "INVENTORY_CATEGORY_TROPHIES", "INVENTORY_CATEGORY_POTIONS" };
static int g_lookups = 0;
static int g_stateCalls = 0;
static long LookupDef(const char* name)
{
    ++g_lookups;
    for (int i = 0; i < 12; ++i)
        if (std::strcmp(g_names[i], name) == 0) return 100 + i;
    return -1;
}
static long GetInventoryCategory(long defIndex) { return defIndex / 10; }
static long GetInventoryTabState(long defIndex) { ++g_stateCalls; return defIndex % 10; }
static long GetTypeFromDefIndexPC(long defIndex)
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
    LookupDef("INVENTORY_CATEGORY_GIFTS");
    LookupDef("INVENTORY_CATEGORY_TROPHIES");
    LookupDef("INVENTORY_CATEGORY_POTIONS");
    long category = GetInventoryCategory(defIndex);
    if (category == melee) return 1;
    else if (category == ranged) return 1;
    else if (category == headwear) return 0;
    else if (category == torso) return 0;
    else if (category == gloves) return 0;
    else if (category == legwear) return 0;
    else if (category == footwear) return 0;
    else if (category == suits) return 0;
    else if (GetInventoryTabState(defIndex) == 0) return 2;
    else if (GetInventoryTabState(defIndex) == 1) return 2;
    else if (GetInventoryTabState(defIndex) == 2) return 2;
    else if (GetInventoryTabState(defIndex) == 3) return 2;
    else if (GetInventoryTabState(defIndex) == 4) return 2;
    else return 2;
}
int main()
{
    int ok = 1;
    if (GetTypeFromDefIndexPC(1010) != 1) ok = 0;   // melee category 101
    if (GetTypeFromDefIndexPC(1025) != 1) ok = 0;   // ranged
    if (GetTypeFromDefIndexPC(1080) != 0) ok = 0;   // suits
    if (GetTypeFromDefIndexPC(1033) != 0) ok = 0;   // headwear
    if (g_lookups != 48) ok = 0;
    if (g_stateCalls != 0) ok = 0;
    if (GetTypeFromDefIndexPC(1000) != 2) ok = 0;   // food -> state 0, one call
    if (g_stateCalls != 1) ok = 0;
    if (GetTypeFromDefIndexPC(1097) != 2) ok = 0;   // gifts, state 7 -> five calls
    if (g_stateCalls != 6) ok = 0;
    if (!ok) { std::printf("0059c447_TEST FAIL\n"); return 1; }
    std::printf("0059c447_TEST PASS\n");
    return 0;
}