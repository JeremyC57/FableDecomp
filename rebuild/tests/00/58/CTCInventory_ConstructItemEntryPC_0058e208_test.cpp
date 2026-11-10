// Behaviour test for 0x0058E208 CTCInventory::ConstructItemEntryPC(defIndex, count, list, owner, actionsList): the PC branch template
// for items with a use-button action (else the plain PC template), recorded in ItemsMenuEntries[defIndex]; usable items get the
// expression-assignment icon and its faded twin (both showing the item's graphic when the def index is non-zero; dropping the icon
// assigns the item, action 253); mesh 2000 (entry, frame), description 2003 (viewport, entry, count, frame), unselected 2015 / 2018,
// visible 2006; the def index is the thing parameter; usable items then get the actions list and the list observes (245) the icon's
// mouse movement while selected and ignores it (249) while inactive.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Comp {
    std::string name; long graphic; std::vector<Comp*> children; std::vector<int> selected, unselected, visible, inactive, dropped;
    long thingParam; std::vector<Comp*> paramComps;
    Comp(const char* n) : name(n), graphic(-1), thingParam(-1) {}
};
struct Inventory {
    std::map<long, Comp*> itemsMenuEntries; std::vector<Comp*> owned;
    ~Inventory() { for (unsigned i = 0; i < owned.size(); ++i) delete owned[i]; }
    Comp* Make(const char* n) { Comp* c = new Comp(n); owned.push_back(c); return c; }
    static int UseButtonActionType(long def) { return def >= 100 ? 1 : 0; }
    static long Graphic(long def) { return def * 10; }
    void ConstructItemEntryPC(long defIndex, long count, Comp* list, Comp* frame, Comp* actionsList)
    {
        Comp* e = Make(UseButtonActionType(defIndex) == 0 ? "PC_MENU_ENTRY_TEMPLATE" : "PC_MENU_ENTRY_BRANCH_TEMPLATE");
        itemsMenuEntries[defIndex] = e;
        Comp* icon = 0;
        if (UseButtonActionType(defIndex)) {
            icon = Make("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON");
            Comp* sprite = Make("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE"); icon->children.push_back(sprite);
            Comp* faded = Make("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_FADED");
            icon->dropped.push_back(253);
            if (defIndex) { sprite->graphic = Graphic(defIndex); faded->graphic = Graphic(defIndex); }
            e->children.push_back(icon); e->children.push_back(faded);
        }
        e->selected.push_back(2000); e->paramComps.push_back(frame);
        e->unselected.push_back(2015);
        e->selected.push_back(2003); e->paramComps.push_back(frame);
        e->unselected.push_back(2018);
        e->visible.push_back(2006);
        e->thingParam = defIndex;
        list->children.push_back(e);
        if (UseButtonActionType(defIndex)) {
            e->children.push_back(actionsList);
            list->selected.push_back(245); list->paramComps.push_back(icon);
            list->inactive.push_back(249); list->paramComps.push_back(icon);
        }
        (void)count;
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); ++fails; } }
int main()
{
    Inventory inv; Comp list("LIST"), frame("PC_UI_FRAME"), actions("ACTIONS");
    inv.ConstructItemEntryPC(7, 2, &list, &frame, &actions);
    Comp* a = inv.itemsMenuEntries[7];
    check(a->name == "PC_MENU_ENTRY_TEMPLATE" && a->children.empty() && list.selected.empty() && a->thingParam == 7, "plain entry");
    check(a->selected.size() == 2 && a->unselected.size() == 2 && a->visible.size() == 1 && a->paramComps[0] == &frame, "plain actions");
    inv.ConstructItemEntryPC(120, 1, &list, &frame, &actions);
    Comp* b = inv.itemsMenuEntries[120];
    check(b->name == "PC_MENU_ENTRY_BRANCH_TEMPLATE" && b->children.size() == 3 && b->children[2] == &actions, "usable entry");
    check(b->children[0]->children[0]->graphic == 1200 && b->children[1]->graphic == 1200 && b->children[0]->dropped[0] == 253, "icon");
    check(list.selected.size() == 1 && list.selected[0] == 245 && list.inactive[0] == 249 && list.paramComps[0] == b->children[0], "observe");
    check(list.children.size() == 2 && list.children[1] == b, "list order");
    if (fails) return 1;
    std::printf("0058e208_TEST PASS\n");
    return 0;
}