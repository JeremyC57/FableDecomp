// Behaviour test for 0x00626110 CTCQuickAccessMenu::ContructItemLists(std::map<long,long>&, CList*, CComponent*, CCountedPointer<CComponent>&):
// a self-contained host model of the row's logic (plain C++98 for g++ and VC7.1, hand-rolled containers).
#include <cstdio>
#include <cstring>

struct Action { long type; long params[4]; int nparams; void* component; };
struct Entry {
    const char* templ; const char* icon; Entry* actions_child; long thing_param;
    Action* visible[2]; int nvisible;
    Entry() : templ(0), icon(0), actions_child(0), thing_param(-1), nvisible(0) {}
};
struct Inventory {
    long quick_access[4];                       // QuickAccessItems: up, down, left, right
    long menu_keys[16]; Entry* menu_values[16]; int nmenu;   // ItemsMenuEntries
    Entry* icons[4];                            // AssignedIcons by slot
    Inventory() : nmenu(0) { for (int i = 0; i < 4; ++i) { quick_access[i] = -1; icons[i] = 0; } }
    void SetMenu(long k, Entry* e) { for (int i = 0; i < nmenu; ++i) if (menu_keys[i] == k) { menu_values[i] = e; return; } menu_keys[nmenu] = k; menu_values[nmenu++] = e; }
};
struct ListModel { Entry* children[16]; int n; ListModel() : n(0) {} };

enum { UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME = 2006 };
static bool UsableItem(long def) { return (def & 1) != 0; }   // GetUseButtonActionType != 0

static Entry entries[16]; static int nentries;
static Action actions[16]; static int nactions;

// keys / values: the std::map<long,long> walked in key order
static void ContructItemLists(Inventory* inventory, const long* keys, const long* counts, int n, ListModel* list, Entry* actions_list)
{
    for (int it = 0; it != n; ++it) {
        long def_index = keys[it];
        long count = counts[it];
        Entry* entry = &entries[nentries++];
        if (!UsableItem(def_index)) entry->templ = "UI_WEAPON_MENU_ENTRY_TEMPLATE";
        else entry->templ = "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS";
        inventory->SetMenu(def_index, entry);
        if (inventory->quick_access[2] == def_index) { entry->icon = "LEFT"; inventory->icons[2] = entry; }
        else if (inventory->quick_access[3] == def_index) { entry->icon = "RIGHT"; inventory->icons[3] = entry; }
        else if (inventory->quick_access[0] == def_index) { entry->icon = "UP"; inventory->icons[0] = entry; }
        else if (inventory->quick_access[1] == def_index) { entry->icon = "DOWN"; inventory->icons[1] = entry; }
        Action* load_name = &actions[nactions++];
        load_name->type = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME; load_name->nparams = 0;
        load_name->params[load_name->nparams++] = def_index;
        load_name->component = entry;
        load_name->params[load_name->nparams++] = count;
        entry->visible[entry->nvisible++] = load_name;
        entry->thing_param = def_index;
        list->children[list->n++] = entry;
        if (UsableItem(def_index)) entry->actions_child = actions_list;
    }
}

static int fails = 0;
static void check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(c) check((c), __LINE__)

int main()
{
    Inventory inv; ListModel list; Entry actions_list;
    inv.quick_access[0] = 30; inv.quick_access[1] = 41; inv.quick_access[2] = 12; inv.quick_access[3] = 7;
    long keys[4] = { 7, 12, 30, 41 }; long counts[4] = { 2, 5, 1, 3 };
    ContructItemLists(&inv, keys, counts, 4, &list, &actions_list);
    CHECK(list.n == 4 && nentries == 4 && nactions == 4);
    CHECK(std::strcmp(entries[0].templ, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS") == 0);
    CHECK(std::strcmp(entries[1].templ, "UI_WEAPON_MENU_ENTRY_TEMPLATE") == 0);
    CHECK(std::strcmp(entries[0].icon, "RIGHT") == 0 && std::strcmp(entries[1].icon, "LEFT") == 0);
    CHECK(std::strcmp(entries[2].icon, "UP") == 0 && std::strcmp(entries[3].icon, "DOWN") == 0);
    CHECK(inv.icons[3] == &entries[0] && inv.icons[2] == &entries[1] && inv.icons[0] == &entries[2] && inv.icons[1] == &entries[3]);
    CHECK(inv.nmenu == 4 && inv.menu_keys[2] == 30 && inv.menu_values[2] == &entries[2]);
    CHECK(actions[1].type == 2006 && actions[1].nparams == 2 && actions[1].params[0] == 12 && actions[1].params[1] == 5 && actions[1].component == &entries[1]);
    CHECK(entries[1].thing_param == 12 && entries[1].nvisible == 1);
    CHECK(entries[0].actions_child == &actions_list && entries[1].actions_child == 0 && entries[3].actions_child == &actions_list);
    // an item in no quick-access slot gets no icon
    Inventory inv2; ListModel list2; long k2[1] = { 99 }; long c2[1] = { 1 };
    ContructItemLists(&inv2, k2, c2, 1, &list2, &actions_list);
    CHECK(list2.n == 1 && entries[4].icon == 0 && inv2.icons[0] == 0 && inv2.icons[2] == 0);
    if (fails) { std::printf("00626110_TEST FAIL\n"); return 1; }
    std::printf("00626110_TEST PASS\n");
    return 0;
}
