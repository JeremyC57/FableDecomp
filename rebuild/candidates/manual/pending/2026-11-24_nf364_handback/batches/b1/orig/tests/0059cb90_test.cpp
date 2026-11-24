// Behaviour test for CTCInventoryTrade::AddWantedItemToList @ 0x0059CB90: one menu entry with five actions (add model and
// load wanted description on select, their removals on unselect, the entry name on visible), appended to the scrolling list
#include <stdio.h>
#include <vector>
enum { ADD_MESH_CHILD = 1, REMOVE_MESH_CHILD, LOAD_WANTED_ITEM_DESC, UNLOAD_WANTED_ITEM_DESC, LOAD_ITEM_MENU_ENTRY_NAME };
enum { ON_SELECTED, ON_UNSELECTED, ON_VISIBLE };
struct Action { int type; int when; std::vector<long> slongs; int components; int strings; };
struct Entry { std::vector<Action> actions; };
static void Add(Entry& e, int type, int when, long a, long b, int components, int strings)
{
    Action act; act.type = type; act.when = when; act.components = components; act.strings = strings;
    if (a != -1) act.slongs.push_back(a);
    if (b != -1) act.slongs.push_back(b);
    e.actions.push_back(act);
}
static void AddWantedItemToList(long itemDefIndex, std::vector<Entry>& list)
{
    Entry entry;
    Add(entry, ADD_MESH_CHILD, ON_SELECTED, itemDefIndex, -1, 1, 1);
    Add(entry, REMOVE_MESH_CHILD, ON_UNSELECTED, -1, -1, 1, 0);
    Add(entry, LOAD_WANTED_ITEM_DESC, ON_SELECTED, itemDefIndex, -1, 2, 0);
    Add(entry, UNLOAD_WANTED_ITEM_DESC, ON_UNSELECTED, -1, -1, 1, 0);
    Add(entry, LOAD_ITEM_MENU_ENTRY_NAME, ON_VISIBLE, itemDefIndex, 1, 1, 0);
    list.push_back(entry);
}
int main()
{
    std::vector<Entry> list;
    AddWantedItemToList(42, list);
    AddWantedItemToList(7, list);
    int ok = list.size() == 2 && list[0].actions.size() == 5;
    const Entry& e = list[0];
    ok = ok && e.actions[0].type == ADD_MESH_CHILD && e.actions[0].slongs[0] == 42 && e.actions[0].strings == 1;
    ok = ok && e.actions[2].type == LOAD_WANTED_ITEM_DESC && e.actions[2].components == 2;
    ok = ok && e.actions[4].when == ON_VISIBLE && e.actions[4].slongs.size() == 2 && e.actions[4].slongs[1] == 1;
    ok = ok && list[1].actions[2].slongs[0] == 7;
    printf(ok ? "0059cb90_TEST PASS\n" : "0059cb90_TEST FAIL\n");
    return ok ? 0 : 1;
}
