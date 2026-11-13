// Behaviour test for 0x00626750 CTCQuickAccessMenu::ContructItemLists(std::vector<std::pair<long,long> >&, CList*, CComponent*,
// CCountedPointer<CComponent>&): a self-contained host model of the row's logic (plain C++98 for g++ and VC7.1).
#include <cstdio>
#include <cstring>

struct Action { long type; long params[4]; int nparams; void* component; };
struct Entry {
    const char* templ; const char* text; const char* icon; Entry* actions_child; long thing_param;
    Action* visible[4]; int nvisible;
    Entry() : templ(0), text(0), icon(0), actions_child(0), thing_param(-1), nvisible(0) {}
};
struct Inventory {
    long quick_access[4];                       // QuickAccessItems: up, down, left, right
    long menu_keys[16]; Entry* menu_values[16]; int nmenu;   // ItemsMenuEntries
    Entry* icons[4];                            // AssignedIcons by slot
    Inventory() : nmenu(0) { for (int i = 0; i < 4; ++i) { quick_access[i] = -1; icons[i] = 0; } }
    void SetMenu(long k, Entry* e) { for (int i = 0; i < nmenu; ++i) if (menu_keys[i] == k) { menu_values[i] = e; return; } menu_keys[nmenu] = k; menu_values[nmenu++] = e; }
};
struct ListModel { Entry* children[16]; int n; ListModel() : n(0) {} };
struct Item { long first; long second; };

enum { UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME = 2006, UI_ACTION_LOAD_QUICK_MENU_EXPRESSIONS_ICON = 2012 };

static Entry entries[16]; static int nentries;
static Action actions[32]; static int nactions;

static void ContructItemLists(Inventory* inventory, const Item* items, unsigned long n, ListModel* list, Entry* actions_list)
{
    for (unsigned long i = 0; i < n; ++i) {
        Entry* entry = &entries[nentries++];
        entry->templ = "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS";
        entry->text = "UI_QUICK_ACCESS_EXPRESSION_MENU_ENTRY_TEXT";
        inventory->SetMenu(items[i].first, entry);
        if (inventory->quick_access[2] == items[i].first) { entry->icon = "LEFT"; inventory->icons[2] = entry; }
        else if (inventory->quick_access[3] == items[i].first) { entry->icon = "RIGHT"; inventory->icons[3] = entry; }
        else if (inventory->quick_access[0] == items[i].first) { entry->icon = "UP"; inventory->icons[0] = entry; }
        else if (inventory->quick_access[1] == items[i].first) { entry->icon = "DOWN"; inventory->icons[1] = entry; }
        Action* load_name = &actions[nactions++];
        load_name->type = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME; load_name->nparams = 0;
        load_name->params[load_name->nparams++] = items[i].first;
        load_name->component = entry;
        load_name->params[load_name->nparams++] = 1;
        entry->visible[entry->nvisible++] = load_name;
        Action* load_icon = &actions[nactions++];
        load_icon->type = UI_ACTION_LOAD_QUICK_MENU_EXPRESSIONS_ICON; load_icon->nparams = 0;
        load_icon->params[load_icon->nparams++] = items[i].first;
        load_icon->component = entry;
        entry->visible[entry->nvisible++] = load_icon;
        entry->thing_param = items[i].first;
        list->children[list->n++] = entry;
        entry->actions_child = actions_list;
    }
}

static int fails = 0;
static void check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(c) check((c), __LINE__)

int main()
{
    Inventory inv; ListModel list; Entry actions_list;
    inv.quick_access[0] = 5; inv.quick_access[3] = 9;
    Item items[3] = { { 9, 4 }, { 5, 2 }, { 11, 1 } };
    ContructItemLists(&inv, items, 3, &list, &actions_list);
    CHECK(list.n == 3 && nentries == 3 && nactions == 6);
    CHECK(std::strcmp(entries[0].icon, "RIGHT") == 0 && std::strcmp(entries[1].icon, "UP") == 0 && entries[2].icon == 0);
    CHECK(inv.icons[3] == &entries[0] && inv.icons[0] == &entries[1] && inv.icons[2] == 0);
    CHECK(inv.nmenu == 3 && inv.menu_keys[0] == 9 && inv.menu_values[1] == &entries[1]);
    CHECK(actions[0].type == 2006 && actions[0].nparams == 2 && actions[0].params[0] == 9 && actions[0].params[1] == 1 && actions[0].component == &entries[0]);
    CHECK(actions[1].type == 2012 && actions[1].nparams == 1 && actions[1].params[0] == 9);
    CHECK(entries[2].nvisible == 2 && entries[2].thing_param == 11 && entries[2].actions_child == &actions_list);
    ListModel empty;
    ContructItemLists(&inv, items, 0, &empty, &actions_list);
    CHECK(empty.n == 0 && nentries == 3);
    if (fails) { std::printf("00626750_TEST FAIL\n"); return 1; }
    std::printf("00626750_TEST PASS\n");
    return 0;
}
