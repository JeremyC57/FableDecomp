// Behaviour test for CTCInventory::SetThingAsQuickAccessItem (0x00584157): only a valid slot and an owned,
// known item are stored; the tutorial is given only when asked and the hero component exists.
#include <cstdio>
// --- test-side copy of the CThing interface lookup (std::bitset presence + sorted {type, component} map) ---
struct CTCBase;
struct CPresenceBits {
    long bits[16];
    bool test(long bit) { return bit >= 0 && bit < 512 && ((bits[bit >> 5] >> (bit & 31)) & 1) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry entries[4];
    void Init() { for (int i = 0; i < 16; ++i) m_present.bits[i] = 0; m_interfaces.first = m_interfaces.last = entries; }
    void Add(long type, void* comp) { m_present.bits[type >> 5] |= 1L << (type & 31); m_interfaces.last->type = type; m_interfaces.last->component = (CTCBase*)comp; ++m_interfaces.last; }
    template<class T> bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
// --- end lookup copy ---
enum ETutorialCategory { TUTORIAL_CATEGORY_NULL, TUTORIAL_CATEGORY_QUICK_ACCESS = 0x17 };
static int g_tutorials = 0; static long g_cat = 0;
struct CTCHero { bool GiveTutorialForCategory(ETutorialCategory category, bool) { g_cat = category; ++g_tutorials; return true; } };
long LookUpInventoryItemDef(long defIndex) { return defIndex == 99 ? 0 : 1; }
struct CLongVector {
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long* begin() { return _M_start; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};
struct CTCInventory {
    CThing* ParentThing;
    CLongVector QuickAccessItems;
    long owned;
    long GetNumberOfItemsOfTypeInInventory(long defIndex) { return defIndex == owned ? 1 : 0; }
    void SetThingAsQuickAccessItem(long item, long slot, bool giveTutorial);
};
void CTCInventory::SetThingAsQuickAccessItem(long item, long slot, bool giveTutorial)
{
    if (slot >= 0 && (unsigned long)slot < QuickAccessItems.size() && item > 0 &&
        GetNumberOfItemsOfTypeInInventory(item) > 0 && LookUpInventoryItemDef(item) != 0) {
        if (giveTutorial) {
            CThing* thing = ParentThing;
            if (thing->HasTC(0x29)) {
                CTCHero* hero = thing->GetTC<CTCHero>(0x29);
                if (hero)
                    hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_QUICK_ACCESS, false);
            }
        }
        QuickAccessItems[slot] = item;
    }
}
int main()
{
    long slots[4] = { 0, 0, 0, 0 };
    CThing thing; thing.Init();
    CTCInventory inv; inv.ParentThing = &thing; inv.QuickAccessItems._M_start = slots; inv.QuickAccessItems._M_finish = inv.QuickAccessItems._M_end_of_storage = slots + 4;
    inv.owned = 7;
    inv.SetThingAsQuickAccessItem(7, 2, true);     // no hero component: stored, no tutorial
    if (slots[2] != 7 || g_tutorials != 0) { std::printf("00584157_TEST FAIL\n"); return 1; }
    CTCHero hero; thing.Add(0x29, &hero);
    inv.SetThingAsQuickAccessItem(7, 3, true);
    if (slots[3] != 7 || g_tutorials != 1 || g_cat != 0x17) { std::printf("00584157_TEST FAIL\n"); return 1; }
    inv.SetThingAsQuickAccessItem(7, 4, true);     // slot out of range
    inv.SetThingAsQuickAccessItem(7, -1, false);
    inv.SetThingAsQuickAccessItem(8, 0, false);    // not owned
    inv.SetThingAsQuickAccessItem(0, 0, false);
    inv.owned = 99; inv.SetThingAsQuickAccessItem(99, 0, false);   // unknown def
    if (slots[0] != 0 || g_tutorials != 1) { std::printf("00584157_TEST FAIL\n"); return 1; }
    inv.owned = 5; inv.SetThingAsQuickAccessItem(5, 0, false);
    if (slots[0] != 5 || g_tutorials != 1) { std::printf("00584157_TEST FAIL\n"); return 1; }
    std::printf("00584157_TEST PASS\n");
    return 0;
}
