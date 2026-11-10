// Behaviour test for 0x005B6A66 CTCInventoryClothing::ConstructClothingListPC: the base inventory is opened, the PC actions list gets
// the "CLOTHING TITLE" header (placed in the frame when there is one), an empty item list returns null, otherwise the PC_SUB_LIST gets
// the title, its offsets for every item, and one entry per item with load-description (viewport, entry, frame) / update-mannequin on
// select, unload-description on unselect, load-name (entry, frame) on visible, the item index as thing parameter and reset on back;
// worn parts use the plain template and make the entry unavailable, the others get the actions list as child.
#include <cstdio>
#include <cstring>

enum { LOAD_DESC = 2002, LOAD_NAME = 2005, UNLOAD_DESC = 2017, UPDATE_MANNEQUIN = 31, RESET_MANNEQUIN = 39 };
struct Action { int type; long longParam; void* params[3]; int nParams; long thingParam; };
struct Component {
    const char* definition;
    const char* name;
    Component* header; Component* positionChildren[4]; int nPositionChildren;
    int offsets; Component* entries[8]; int nEntries;
    Action onSelected[2]; int nSelected; Action onUnselected; Action onVisible; Action onBack;
    long thingParam; bool unavailable; Component* child;
};
static Component g_pool[32];
static int g_nPool;
static Component* Create(const char* definition)
{
    Component* c = &g_pool[g_nPool++];
    std::memset(c, 0, sizeof(*c));
    c->definition = definition;
    return c;
}
struct Registered { int part; Component* entry; };
struct Clothing {
    int opened;
    Registered registered[8]; int nRegistered;
    Component* ConstructClothingListPC(const long* defs, unsigned long count, Component* viewport, Component* frame, int part, bool worn)
    {
        ++opened;
        Component* actions = Create("PC_UI_LIST_CLOTHING_ACTIONS");
        Component* title = Create("PC_TITLE");
        title->name = "CLOTHING TITLE";
        actions->header = title;
        if (frame) frame->positionChildren[frame->nPositionChildren++] = title;
        if (count == 0)
            return 0;
        Component* list = Create("PC_SUB_LIST");
        Component* listTitle = Create("PC_TITLE");
        listTitle->name = "CLOTHING TITLE";
        list->header = listTitle;
        if (frame) frame->positionChildren[frame->nPositionChildren++] = listTitle;
        list->offsets = (int)count;
        for (unsigned long i = 0; i < count; ++i) {
            long defIndex = defs[i];
            Component* entry = Create(worn ? "PC_MENU_ENTRY_TEMPLATE" : "PC_MENU_ENTRY_BRANCH_TEMPLATE");
            Action load = { LOAD_DESC, defIndex, { viewport, entry, frame }, 3, 0 };
            entry->onSelected[entry->nSelected++] = load;
            Action unload = { UNLOAD_DESC, 0, { entry, 0, 0 }, 1, 0 };
            entry->onUnselected = unload;
            Action name = { LOAD_NAME, defIndex, { entry, frame, 0 }, 2, 0 };
            entry->onVisible = name;
            entry->thingParam = defIndex;
            Action wear = { UPDATE_MANNEQUIN, 0, { 0, 0, 0 }, 0, defIndex };
            entry->onSelected[entry->nSelected++] = wear;
            Action reset = { RESET_MANNEQUIN, 0, { 0, 0, 0 }, 0, 0 };
            entry->onBack = reset;
            list->entries[list->nEntries++] = entry;
            if (worn) entry->unavailable = true; else entry->child = actions;
            Registered r = { part, entry };
            registered[nRegistered++] = r;
        }
        return list;
    }
};

int main()
{
    Clothing c; std::memset(&c, 0, sizeof(c));
    Component* viewport = Create("PC_DESCRIPTION_VIEWPORT");
    Component* frame = Create("PC_UI_FRAME");
    long defs[3] = { 40, 41, 42 };
    Component* none = c.ConstructClothingListPC(defs, 0, viewport, frame, 2, false);
    bool emptyOk = none == 0 && c.opened == 1 && frame->nPositionChildren == 1 && g_nPool == 4;
    Component* list = c.ConstructClothingListPC(defs, 3, viewport, frame, 5, false);
    Component* first = c.registered[0].entry;
    bool listOk = list != 0 && std::strcmp(list->definition, "PC_SUB_LIST") == 0 && list->offsets == 3 && list->nEntries == 3
        && std::strcmp(list->header->name, "CLOTHING TITLE") == 0 && frame->nPositionChildren == 3
        && std::strcmp(first->definition, "PC_MENU_ENTRY_BRANCH_TEMPLATE") == 0 && first->nSelected == 2
        && first->onSelected[0].type == LOAD_DESC && first->onSelected[0].longParam == 40 && first->onSelected[0].params[0] == viewport
        && first->onSelected[0].params[1] == first && first->onSelected[0].params[2] == frame
        && first->onUnselected.type == UNLOAD_DESC && first->onVisible.type == LOAD_NAME && first->onVisible.params[1] == frame
        && first->thingParam == 40 && first->onSelected[1].type == UPDATE_MANNEQUIN && first->onSelected[1].thingParam == 40
        && first->onBack.type == RESET_MANNEQUIN && first->child != 0 && !first->unavailable
        && std::strcmp(first->child->definition, "PC_UI_LIST_CLOTHING_ACTIONS") == 0 && c.registered[2].entry->thingParam == 42;
    Component* plain = c.ConstructClothingListPC(defs, 1, viewport, 0, 1, true);
    Component* worn = c.registered[3].entry;
    bool wornOk = plain != 0 && c.nRegistered == 4 && c.registered[3].part == 1 && worn->unavailable && worn->child == 0
        && std::strcmp(worn->definition, "PC_MENU_ENTRY_TEMPLATE") == 0 && worn->onSelected[0].params[2] == 0 && c.opened == 3;
    if (!(emptyOk && listOk && wornOk)) { std::printf("005b6a66_TEST FAIL %d %d %d\n", emptyOk, listOk, wornOk); return 1; }
    std::printf("005b6a66_TEST PASS\n");
    return 0;
}