// Behaviour test for CTCInventoryTrade::AddWantedItemToListPC @ 0x0059D270: one PC menu entry per wanted item, with five actions
// in retail order -- ADD_MESH_CHILD (item, entry, "PC_MODEL", frame) and LOAD_WANTED_ITEM_DESC (item, viewport, entry, frame) on
// select, REMOVE_MESH_CHILD (entry) and UNLOAD_WANTED_ITEM_DESC (entry) on unselect, LOAD_ITEM_MENU_ENTRY_NAME (item, entry, 1) on
// visible -- and the entry is added to the list dynamically (scrolling, offsets set).
#include <cstdio>
#include <string>
#include <vector>
enum { ADD_MESH_CHILD = 2000, LOAD_ITEM_MENU_ENTRY_NAME = 2006, LOAD_WANTED_ITEM_DESC = 2011, REMOVE_MESH_CHILD = 2015,
       UNLOAD_WANTED_ITEM_DESC = 2023 };
struct Param { char kind; long value; const void* ptr; std::string text; };
struct Action { long type; std::vector<Param> params; };
struct Entry {
    std::string name, templ, button;
    std::vector<Action> onSelected, onUnselected, onVisible;
};
struct List { std::vector<Entry*> scrolling; bool dynamic; };
static Param L(long v) { Param p; p.kind = 'l'; p.value = v; p.ptr = 0; return p; }
static Param P(const void* v) { Param p; p.kind = 'p'; p.value = 0; p.ptr = v; return p; }
static Param S(const char* t) { Param p; p.kind = 's'; p.value = 0; p.ptr = 0; p.text = t; return p; }
static void AddWantedItemToListPC(const void* frame, long item, List* list, const void* viewport, std::vector<Entry*>& owned)
{
    Entry* entry = new Entry;
    owned.push_back(entry);
    entry->name = "INVALID NAME"; entry->templ = "PC_MENU_ENTRY_TEMPLATE"; entry->button = "PC_BUTTON_BASTARD";
    Action add; add.type = ADD_MESH_CHILD;
    add.params.push_back(L(item)); add.params.push_back(P(entry)); add.params.push_back(S("PC_MODEL")); add.params.push_back(P(frame));
    entry->onSelected.push_back(add);
    Action remove; remove.type = REMOVE_MESH_CHILD; remove.params.push_back(P(entry));
    entry->onUnselected.push_back(remove);
    Action load; load.type = LOAD_WANTED_ITEM_DESC;
    load.params.push_back(L(item)); load.params.push_back(P(viewport)); load.params.push_back(P(entry)); load.params.push_back(P(frame));
    entry->onSelected.push_back(load);
    Action unload; unload.type = UNLOAD_WANTED_ITEM_DESC; unload.params.push_back(P(entry));
    entry->onUnselected.push_back(unload);
    Action name; name.type = LOAD_ITEM_MENU_ENTRY_NAME;
    name.params.push_back(L(item)); name.params.push_back(P(entry)); name.params.push_back(L(1));
    entry->onVisible.push_back(name);
    list->scrolling.push_back(entry);
    list->dynamic = true;
}
int main()
{
    int ok = 1;
    int frame = 0, viewport = 0;
    List list; list.dynamic = false;
    std::vector<Entry*> owned;
    AddWantedItemToListPC(&frame, 77, &list, &viewport, owned);
    AddWantedItemToListPC(&frame, 78, &list, &viewport, owned);
    if (list.scrolling.size() != 2 || !list.dynamic) ok = 0;
    const Entry* e = list.scrolling[0];
    if (e->templ != "PC_MENU_ENTRY_TEMPLATE" || e->button != "PC_BUTTON_BASTARD" || e->name != "INVALID NAME") ok = 0;
    if (e->onSelected.size() != 2 || e->onUnselected.size() != 2 || e->onVisible.size() != 1) ok = 0;
    if (ok) {
        const Action& a = e->onSelected[0];
        if (a.type != ADD_MESH_CHILD || a.params.size() != 4 || a.params[0].value != 77 || a.params[1].ptr != e ||
            a.params[2].text != "PC_MODEL" || a.params[3].ptr != &frame) ok = 0;
        const Action& d = e->onSelected[1];
        if (d.type != LOAD_WANTED_ITEM_DESC || d.params[1].ptr != &viewport || d.params[2].ptr != e || d.params[3].ptr != &frame) ok = 0;
        if (e->onUnselected[0].type != REMOVE_MESH_CHILD || e->onUnselected[1].type != UNLOAD_WANTED_ITEM_DESC) ok = 0;
        const Action& n = e->onVisible[0];
        if (n.type != LOAD_ITEM_MENU_ENTRY_NAME || n.params[0].value != 77 || n.params[2].value != 1) ok = 0;
        if (list.scrolling[1]->onVisible[0].params[0].value != 78) ok = 0;
    }
    for (unsigned int i = 0; i < owned.size(); ++i) delete owned[i];
    if (!ok) { std::printf("0059d270_TEST FAIL\n"); return 1; }
    std::printf("0059d270_TEST PASS\n");
    return 0;
}