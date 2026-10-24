// Behaviour test for 0x005B70B8 CTCInventoryClothing::ConstructClothingList: the actions list gets back / select / title
// headers, an empty item list returns null, otherwise the sub-list gets the same three headers, one entry per item with
// select (viewport, entry), focus, unfocus, equip and remove actions, and each entry is cleared or given the actions list.
#include <cstdio>
#include <cstring>

struct Component {
    const char* definition;
    const char* headers[4]; int nHeaders;
    const char* name;
    int itemCount;
    int entries[8]; int nEntries;
    int selected[4]; int nSelected;
    int focus; int unfocus; int special;
    long userValue;
    bool cleared; Component* child;
    void* selectParams[2];
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
    Registered registered[8]; int nRegistered;
    Component* ConstructClothingList(const long* defs, int count, void* viewport, int part, bool plainEntries)
    {
        Component* actionsList = Create("UI_LIST_CLOTHING_ACTIONS");
        Component* back = Create("UI_BACK_FOR_LIST");
        Component* select = Create("UI_SELECT_FOR_LIST");
        actionsList->headers[actionsList->nHeaders++] = back->definition;
        actionsList->headers[actionsList->nHeaders++] = select->definition;
        Component* title = Create("UI_TITLE_NEW");
        title->name = "CLOTHING TITLE";
        actionsList->headers[actionsList->nHeaders++] = title->definition;
        Component* subList = 0;
        if (count != 0) {
            subList = Create("UI_SUB_LIST");
            subList->headers[subList->nHeaders++] = Create("UI_BACK_FOR_LIST")->definition;
            subList->headers[subList->nHeaders++] = Create("UI_SELECT_FOR_LIST")->definition;
            Component* subTitle = Create("UI_TITLE_NEW");
            subTitle->name = "CLOTHING TITLE";
            subList->headers[subList->nHeaders++] = subTitle->definition;
            subList->itemCount = count;
            for (unsigned long i = 0; i < (unsigned long)count; ++i) {
                long defIndex = defs[i];
                Component* entry = Create("UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW");
                entry->selectParams[0] = viewport;
                entry->selectParams[1] = entry;
                entry->selected[entry->nSelected++] = 0x7d2;
                entry->focus = 0x7e1;
                entry->unfocus = 0x7d5;
                entry->userValue = defIndex;
                entry->selected[entry->nSelected++] = 0x1f;
                entry->special = 0x27;
                subList->entries[subList->nEntries++] = (int)defIndex;
                if (plainEntries) entry->cleared = true; else entry->child = actionsList;
                Registered r = { part, entry };
                registered[nRegistered++] = r;
            }
        }
        return subList;
    }
};

int main()
{
    Clothing c; std::memset(&c, 0, sizeof(c));
    int viewport;
    long defs[3] = { 40, 41, 42 };
    Component* none = c.ConstructClothingList(defs, 0, &viewport, 2, false);
    int afterEmpty = g_nPool;
    Component* list = c.ConstructClothingList(defs, 3, &viewport, 5, false);
    Component* plain = c.ConstructClothingList(defs, 1, &viewport, 1, true);
    Component* firstEntry = c.registered[0].entry;
    bool ok = none == 0 && afterEmpty == 4 && list != 0 && list->nHeaders == 3 && list->itemCount == 3 && list->nEntries == 3
        && list->entries[2] == 42 && std::strcmp(list->definition, "UI_SUB_LIST") == 0
        && c.nRegistered == 4 && c.registered[0].part == 5 && firstEntry->nSelected == 2 && firstEntry->selected[0] == 0x7d2
        && firstEntry->selectParams[0] == &viewport && firstEntry->selectParams[1] == firstEntry && firstEntry->userValue == 40
        && firstEntry->child != 0 && std::strcmp(firstEntry->child->definition, "UI_LIST_CLOTHING_ACTIONS") == 0 && !firstEntry->cleared
        && plain != 0 && c.registered[3].entry->cleared && c.registered[3].entry->child == 0 && c.registered[3].part == 1;
    if (!ok) { std::printf("005b70b8_TEST FAIL\n"); return 1; }
    std::printf("005b70b8_TEST PASS\n");
    return 0;
}