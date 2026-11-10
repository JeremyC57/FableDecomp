// Behaviour test for CTCInventoryTrade::AddWantedItemToListPC @ 0x0059D270: the PC wanted entry gets five actions in retail
// order (select 0x7d0, focus 0x7df, view 0x7db, focus 0x7e7, unfocus 0x7d6), the frame is an extra target of select and view,
// and the entry goes to the TOP of the list.
#include <cstdio>
#include <vector>
struct Param { int kind; long value; const void* target; };   // kind 0 = long, 1 = string, 2 = component target
struct Action { long type; std::vector<Param> params; };
struct Component { std::vector<Action> selected, focus, unfocus; const char* templ; };
struct List { std::vector<Component*> entries; void AddEntry(Component* c) { entries.push_back(c); }
              void AddEntryAtTop(Component* c) { entries.insert(entries.begin(), c); } };
static Param L(long v) { Param p; p.kind = 0; p.value = v; p.target = 0; return p; }
static Param S() { Param p; p.kind = 1; p.value = 0; p.target = 0; return p; }
static Param T(const void* t) { Param p; p.kind = 2; p.value = 0; p.target = t; return p; }
static void AddWantedItemToListPC(Component* frame, long defIndex, List* list, Component* viewport, Component* entry)
{
    entry->templ = "PC_MENU_ENTRY_TEMPLATE";
    Action select; select.type = 0x7d0; select.params.push_back(L(defIndex)); select.params.push_back(T(entry));
    select.params.push_back(S()); select.params.push_back(T(frame)); entry->selected.push_back(select);
    Action focus; focus.type = 0x7df; focus.params.push_back(T(entry)); entry->focus.push_back(focus);
    Action view; view.type = 0x7db; view.params.push_back(L(defIndex)); view.params.push_back(T(viewport));
    view.params.push_back(T(entry)); view.params.push_back(T(frame)); entry->selected.push_back(view);
    Action focus2; focus2.type = 0x7e7; focus2.params.push_back(T(entry)); entry->focus.push_back(focus2);
    Action unfocus; unfocus.type = 0x7d6; unfocus.params.push_back(L(defIndex)); unfocus.params.push_back(T(entry));
    unfocus.params.push_back(L(1)); entry->unfocus.push_back(unfocus);
    list->AddEntryAtTop(entry);
}
int main()
{
    int ok = 1;
    Component frame, viewport, old, e;
    List list; list.AddEntry(&old);
    AddWantedItemToListPC(&frame, 77, &list, &viewport, &e);
    if (list.entries.size() != 2 || list.entries[0] != &e) ok = 0;
    if (e.selected.size() != 2 || e.focus.size() != 2 || e.unfocus.size() != 1) ok = 0;
    if (e.selected[0].type != 0x7d0 || e.selected[0].params[0].value != 77 || e.selected[0].params[3].target != &frame) ok = 0;
    if (e.selected[1].type != 0x7db || e.selected[1].params[1].target != &viewport || e.selected[1].params[3].target != &frame) ok = 0;
    if (e.focus[0].type != 0x7df || e.focus[1].type != 0x7e7) ok = 0;
    if (e.unfocus[0].type != 0x7d6 || e.unfocus[0].params[2].value != 1) ok = 0;
    if (!ok) { std::printf("0059d270_TEST FAIL\n"); return 1; }
    std::printf("0059d270_TEST PASS\n");
    return 0;
}
