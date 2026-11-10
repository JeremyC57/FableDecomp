// Behaviour test of CTCInventoryWeapons::FinalConstructWeaponItemEntries (005c3e91): one menu entry per weapon item appended to the
// list as scrolling children; the entry of the wielded melee (else ranged) weapon becomes the list default at offset + i and records
// the assigned entry / list (and an assigned icon when the item has a graphic); every entry gets, on selected, the add-mesh (2000:
// def index, entry, L"UI_WEAPON_MODEL_NEW") and load-description (2001: def index, viewport, entry) actions, on unselected the
// remove-mesh (2015) and unload-description (2016) actions, on visible the load-name action (2004: def index, entry, title,
// augmentation entry, weapons-usable flag, wield entry), and finally the def index and itself as its own action parameters.
#include <cstdio>
#include <string>
#include <utility>
#include <vector>
struct CAction { long ActionType; std::vector<std::string> Params; };
struct CEntry {
    long Def; long Graphic;
    std::vector<CAction> OnSelected, OnUnselected, OnVisible;
    std::vector<long> ThingParams; std::vector<std::string> UIParams;
    std::vector<std::string> Children;
};
struct CList { std::vector<CEntry*> Scrolling; long Default; CList() : Default(-1) { } };
struct CInventory {
    bool MeleeAlive, RangedAlive; long MeleeDef, RangedDef; bool HasHero, HeroWeaponsUsable;
    CEntry* AssignedMelee; CEntry* AssignedRanged; CList* MeleeList; CList* RangedList; bool MeleeIcon, RangedIcon;
};
static long GraphicOf(long def) { return def == 7 ? 0 : def * 100; }       // def 7 has no graphic
static std::string L2S(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static void FinalConstructWeaponItemEntries(CInventory& inv, std::vector<std::pair<long, long> >& items, CList* list, unsigned long offset)
{
    bool weaponsUsable = true;
    if (inv.HasHero && !inv.HeroWeaponsUsable)
        weaponsUsable = false;
    for (unsigned long i = 0; i < items.size(); ++i) {
        long def = items[i].first;
        CEntry* entry = new CEntry; entry->Def = def; entry->Graphic = 0;
        if (inv.MeleeAlive && def == inv.MeleeDef) {
            list->Default = (long)(offset + i);
            inv.AssignedMelee = entry; inv.MeleeList = list;
            if (GraphicOf(def) != 0) { entry->Children.push_back("icon " + L2S(GraphicOf(def))); inv.MeleeIcon = true; }
        } else if (inv.RangedAlive && def == inv.RangedDef) {
            inv.AssignedRanged = entry; inv.RangedList = list;
            list->Default = (long)(offset + i);
            if (GraphicOf(def) != 0) { entry->Children.push_back("icon " + L2S(GraphicOf(def))); inv.RangedIcon = true; }
        }
        CAction a;
        a.ActionType = 2000; a.Params.clear(); a.Params.push_back(L2S(def)); a.Params.push_back("entry"); a.Params.push_back("UI_WEAPON_MODEL_NEW");
        entry->OnSelected.push_back(a);
        a.ActionType = 2015; a.Params.clear(); a.Params.push_back("entry"); entry->OnUnselected.push_back(a);
        a.ActionType = 2001; a.Params.clear(); a.Params.push_back(L2S(def)); a.Params.push_back("viewport"); a.Params.push_back("entry");
        entry->OnSelected.push_back(a);
        a.ActionType = 2016; a.Params.clear(); a.Params.push_back("entry"); entry->OnUnselected.push_back(a);
        a.ActionType = 2004; a.Params.clear(); a.Params.push_back(L2S(def)); a.Params.push_back("entry"); a.Params.push_back("title");
        a.Params.push_back("augmentations"); a.Params.push_back(weaponsUsable ? "1" : "0"); a.Params.push_back("wield");
        entry->OnVisible.push_back(a);
        list->Scrolling.push_back(entry);
        entry->ThingParams.push_back(def);
        entry->UIParams.push_back("entry");
    }
}
int main()
{
    CInventory inv = { true, true, 5, 7, true, false, 0, 0, 0, 0, false, false };
    std::vector<std::pair<long, long> > items;
    items.push_back(std::make_pair(3L, 1L)); items.push_back(std::make_pair(5L, 1L)); items.push_back(std::make_pair(7L, 2L));
    CList list;
    FinalConstructWeaponItemEntries(inv, items, &list, 4);
    int bad = 0;
    bad |= list.Scrolling.size() != 3;
    bad |= list.Default != 6;                                    // the ranged weapon (def 7, i = 2) set it last: 4 + 2
    bad |= inv.AssignedMelee != list.Scrolling[1] || inv.MeleeList != &list || !inv.MeleeIcon;
    bad |= inv.AssignedRanged != list.Scrolling[2] || inv.RangedList != &list || inv.RangedIcon;   // def 7: no graphic, no icon
    bad |= list.Scrolling[1]->Children.size() != 1 || list.Scrolling[1]->Children[0] != "icon 500";
    for (unsigned i = 0; i < 3; ++i) {
        CEntry* e = list.Scrolling[i];
        bad |= e->OnSelected.size() != 2 || e->OnSelected[0].ActionType != 2000 || e->OnSelected[1].ActionType != 2001;
        bad |= e->OnUnselected.size() != 2 || e->OnUnselected[0].ActionType != 2015 || e->OnUnselected[1].ActionType != 2016;
        bad |= e->OnVisible.size() != 1 || e->OnVisible[0].Params.size() != 6 || e->OnVisible[0].Params[4] != "0";   // hero may not use weapons
        bad |= e->ThingParams.size() != 1 || e->ThingParams[0] != items[i].first;
    }
    // no hero component: weapons usable; nothing wielded: no default
    CInventory inv2 = { false, false, 0, 0, false, false, 0, 0, 0, 0, false, false };
    CList list2;
    FinalConstructWeaponItemEntries(inv2, items, &list2, 0);
    bad |= list2.Default != -1 || inv2.AssignedMelee != 0 || list2.Scrolling[0]->OnVisible[0].Params[4] != "1";
    if (bad) { std::printf("005c3e91_TEST FAIL\n"); return 1; }
    std::printf("005c3e91_TEST PASS\n");
    return 0;
}