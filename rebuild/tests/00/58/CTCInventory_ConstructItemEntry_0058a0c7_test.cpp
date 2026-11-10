// Behaviour test for 0x0058A0C7 CTCInventory::ConstructItemEntry(defIndex, count, list, owner, actionsList): the entry uses the branch
// template when the item has a use-button action (else the plain one) and is recorded in ItemsMenuEntries[defIndex]; usable items get
// the select marker child; the first quick-access slot (left 2, right 3, up 0, down 1) holding the item adds that icon, recorded in
// AssignedIcons[slot]; selected actions 2000 (mesh) and 2003 (description: def, text area, entry, count), unselected 2015 / 2018,
// visible 2006 (def, entry, count); the def index is the thing parameter, the entry joins the list, usable items get the actions list.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Entry {
    std::string templ; std::vector<std::string> children; std::vector<int> selected, unselected, visible; long thingParam;
    std::vector<long> descParams;
    Entry() : thingParam(-1) {}
};
struct Inventory {
    std::vector<long> quick; std::map<long, Entry*> itemsMenuEntries; std::map<long, std::string> assignedIcons;
    std::vector<Entry*> list; std::vector<Entry*> owned;
    ~Inventory() { for (unsigned i = 0; i < owned.size(); ++i) delete owned[i]; }
    static int UseButtonActionType(long def) { return def >= 100 ? 1 : 0; }
    void ConstructItemEntry(long defIndex, long count)
    {
        Entry* e = new Entry; owned.push_back(e);
        e->templ = UseButtonActionType(defIndex) == 0 ? "UI_WEAPON_MENU_ENTRY_TEMPLATE" : "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE";
        itemsMenuEntries[defIndex] = e;
        if (UseButtonActionType(defIndex)) e->children.push_back("UI_SELECT_FOR_MENU_ENTRY");
        static const int order[4] = { 2, 3, 0, 1 };
        static const char* const icons[4] = { "UI_ASSIGNED_LEFT_ITEM_ICON", "UI_ASSIGNED_RIGHT_ITEM_ICON", "UI_ASSIGNED_UP_ITEM_ICON",
                                              "UI_ASSIGNED_DOWN_ITEM_ICON" };
        for (int k = 0; k < 4; ++k)
            if (quick[order[k]] == defIndex) { e->children.push_back(icons[k]); assignedIcons[order[k]] = icons[k]; break; }
        e->selected.push_back(2000); e->unselected.push_back(2015);
        e->selected.push_back(2003); e->descParams.push_back(defIndex); e->descParams.push_back(count);
        e->unselected.push_back(2018); e->visible.push_back(2006);
        e->thingParam = defIndex;
        list.push_back(e);
        if (UseButtonActionType(defIndex)) e->children.push_back("ACTIONS_LIST");
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); ++fails; } }
int main()
{
    Inventory inv; inv.quick.resize(4, -1); inv.quick[0] = 7; inv.quick[3] = 7; inv.quick[1] = 120;
    inv.ConstructItemEntry(7, 3);      // plain item in the up and right slots: right (3) wins
    Entry* a = inv.itemsMenuEntries[7];
    check(a && a->templ == "UI_WEAPON_MENU_ENTRY_TEMPLATE" && a->children.size() == 1 && a->children[0] == "UI_ASSIGNED_RIGHT_ITEM_ICON", "plain entry");
    check(inv.assignedIcons.size() == 1 && inv.assignedIcons.count(3) == 1, "right icon recorded");
    check(a->selected.size() == 2 && a->selected[1] == 2003 && a->descParams[1] == 3 && a->visible[0] == 2006 && a->thingParam == 7, "actions");
    inv.ConstructItemEntry(120, 1);    // usable item in the down slot
    Entry* b = inv.itemsMenuEntries[120];
    check(b->templ == "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE" && b->children.size() == 3 && b->children[0] == "UI_SELECT_FOR_MENU_ENTRY"
          && b->children[1] == "UI_ASSIGNED_DOWN_ITEM_ICON" && b->children[2] == "ACTIONS_LIST", "usable entry");
    check(inv.list.size() == 2 && inv.list[1] == b && inv.assignedIcons.count(1) == 1, "list order");
    inv.ConstructItemEntry(5, 1);      // unassigned plain item
    check(inv.itemsMenuEntries[5]->children.empty() && inv.assignedIcons.size() == 2, "no icon");
    if (fails) return 1;
    std::printf("0058a0c7_TEST PASS\n");
    return 0;
}