// Behaviour test of CTCInventoryWeapons::ConstructWeaponItemEntries (0x005C66B2): the items are split by the first item's kind --
// melee: light (class 1) weapons, then the rest, the second group offset by the first group's size; ranged: type-3 weapons, then
// the rest, the second group given its own size (as retail computes it) -- one wield entry (melee or ranged) for both calls.
#include <cstdio>
#include <vector>
#include <utility>
typedef std::vector<std::pair<long, long> > CItemVector;
struct SWeaponDef { int Class; int Type; };
static SWeaponDef g_defs[16];
static bool IsMeleeWeaponClass(int c) { return c == 1 || c == 2; }
struct SCall { CItemVector items; bool meleeWield; unsigned long offset; };
static std::vector<SCall> g_calls;
static int g_wieldBuilt = 0;
struct CWieldEntry { bool melee; };
static CWieldEntry ConstructWieldEntry(bool melee) { ++g_wieldBuilt; CWieldEntry e = { melee }; return e; }
static void FinalConstructWeaponItemEntries(const CItemVector& items, const CWieldEntry& wield, unsigned long offset)
{
    SCall c; c.items = items; c.meleeWield = wield.melee; c.offset = offset; g_calls.push_back(c);
}
static void ConstructWeaponItemEntries(CItemVector& items)
{
    if (items.empty())
        return;
    if (IsMeleeWeaponClass(g_defs[items[0].first].Class)) {
        CItemVector lightItems, otherItems;
        for (unsigned long i = 0; i < items.size(); ++i) {
            if (g_defs[items[i].first].Class == 1) lightItems.push_back(items[i]); else otherItems.push_back(items[i]);
        }
        CWieldEntry wield = ConstructWieldEntry(true);
        FinalConstructWeaponItemEntries(lightItems, wield, 0);
        FinalConstructWeaponItemEntries(otherItems, wield, lightItems.size());
    } else {
        CItemVector typeItems, otherItems;
        for (unsigned long i = 0; i < items.size(); ++i) {
            if (g_defs[items[i].first].Type == 3) typeItems.push_back(items[i]); else otherItems.push_back(items[i]);
        }
        CWieldEntry wield = ConstructWieldEntry(false);
        FinalConstructWeaponItemEntries(typeItems, wield, 0);
        FinalConstructWeaponItemEntries(otherItems, wield, otherItems.size());
    }
}
int main()
{
    g_defs[1].Class = 1; g_defs[2].Class = 2; g_defs[3].Class = 1;
    g_defs[4].Class = 3; g_defs[4].Type = 3; g_defs[5].Class = 3; g_defs[5].Type = 4; g_defs[6].Class = 3; g_defs[6].Type = 4;
    CItemVector melee; melee.push_back(std::make_pair(2L, 1L)); melee.push_back(std::make_pair(1L, 2L)); melee.push_back(std::make_pair(3L, 1L));
    ConstructWeaponItemEntries(melee);
    int bad = !(g_calls.size() == 2 && g_calls[0].items.size() == 2 && g_calls[0].items[0].first == 1 && g_calls[0].items[1].first == 3
                && g_calls[0].offset == 0 && g_calls[1].items.size() == 1 && g_calls[1].items[0].first == 2 && g_calls[1].offset == 2
                && g_calls[0].meleeWield && g_wieldBuilt == 1);
    g_calls.clear();
    CItemVector ranged; ranged.push_back(std::make_pair(5L, 1L)); ranged.push_back(std::make_pair(4L, 1L)); ranged.push_back(std::make_pair(6L, 3L));
    ConstructWeaponItemEntries(ranged);
    bad |= !(g_calls.size() == 2 && g_calls[0].items.size() == 1 && g_calls[0].items[0].first == 4 && g_calls[0].offset == 0
             && g_calls[1].items.size() == 2 && g_calls[1].offset == 2 && !g_calls[1].meleeWield && g_wieldBuilt == 2);
    g_calls.clear();
    CItemVector none;
    ConstructWeaponItemEntries(none);
    bad |= !(g_calls.empty() && g_wieldBuilt == 2);
    if (bad) { std::printf("005c66b2_TEST FAIL\n"); return 1; }
    std::printf("005c66b2_TEST PASS\n");
    return 0;
}