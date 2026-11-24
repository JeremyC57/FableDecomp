// Behaviour test for 0x0059F51E CTCInventoryTrade::AddSellItemToListPC (PC twin: PC templates, owner params): the branch/plain template choice, the ItemsToSell
// record, the seven actions (types, long parameters, extra params) and the list insertion side.
#include <cstdio>
#include <cstring>

struct Component {
    const char* definition;
    int cleared;
    int selected[4]; int nSelected;     // action types added through AddSelectedAction
    int secondary;                      // AddSecondaryAction type
    int focus[4]; int nFocus;
    int unfocus;
    Component* userParam;
};
struct Action {
    long type;
    long longs[4]; int nLongs;
    void* params[4]; int nParams;
    int strings;
};
static Component g_components[4];
static int g_nComponents;
static Action g_actions[16];
static int g_nActions;
static Component* CreateMenuEntry(const char* definition)
{
    Component* c = &g_components[g_nComponents++];
    std::memset(c, 0, sizeof(*c));
    c->definition = definition;
    return c;
}
static Action* NewAction() { Action* a = &g_actions[g_nActions++]; std::memset(a, 0, sizeof(*a)); return a; }
struct SellRecord { Component* entry; long defIndex; long count; };
struct Trade {
    bool shopHasDefault;
    SellRecord records[4]; int nRecords;
    Component* top[4]; int nTop;
    Component* bottom[4]; int nBottom;
    bool AddSellItemToListPC(void* owner, long defIndex, long count, void* viewport, bool addToTop)
    {
        bool isBranch = true;
        Component* entry;
        if (shopHasDefault) {
            entry = CreateMenuEntry("PC_MENU_ENTRY_BRANCH_TEMPLATE");
        } else {
            isBranch = false;
            entry = CreateMenuEntry("PC_MENU_ENTRY_TEMPLATE");
            entry->cleared = 1;
        }
        SellRecord r = { entry, defIndex, count };
        records[nRecords++] = r;
        Action* select = NewAction(); select->type = 0x7d0; select->longs[select->nLongs++] = defIndex;
        select->params[select->nParams++] = entry; select->strings = 1; select->params[select->nParams++] = owner; entry->selected[entry->nSelected++] = 0x7d0;
        Action* focus = NewAction(); focus->type = 0x7df; focus->params[focus->nParams++] = entry; entry->focus[entry->nFocus++] = 0x7df;
        Action* view = NewAction(); view->type = 0x7d7; view->longs[view->nLongs++] = defIndex;
        view->params[view->nParams++] = viewport; view->params[view->nParams++] = entry; view->params[view->nParams++] = owner; entry->selected[entry->nSelected++] = 0x7d7;
        Action* focusView = NewAction(); focusView->type = 0x7e3; focusView->params[focusView->nParams++] = entry; entry->focus[entry->nFocus++] = 0x7e3;
        Action* sell = NewAction(); sell->type = 0x7d8; sell->longs[sell->nLongs++] = defIndex; sell->params[sell->nParams++] = entry;
        entry->secondary = 0x7d8;
        Action* focusSell = NewAction(); focusSell->type = 0x7e4; focusSell->params[focusSell->nParams++] = entry; entry->focus[entry->nFocus++] = 0x7e4;
        Action* unfocus = NewAction(); unfocus->type = 0x7d6; unfocus->longs[unfocus->nLongs++] = defIndex;
        unfocus->params[unfocus->nParams++] = entry; unfocus->longs[unfocus->nLongs++] = count; entry->unfocus = 0x7d6;
        entry->userParam = entry;
        if (addToTop) top[nTop++] = entry; else bottom[nBottom++] = entry;
        return isBranch;
    }
};

int main()
{
    Trade t; std::memset(&t, 0, sizeof(t));
    int viewport; int owner;
    t.shopHasDefault = true;
    bool a = t.AddSellItemToListPC(&owner, 42, 3, &viewport, false);
    t.shopHasDefault = false;
    bool b = t.AddSellItemToListPC(&owner, 7, 1, &viewport, true);
    bool ok = a && !b && t.nRecords == 2 && t.records[0].defIndex == 42 && t.records[0].count == 3
        && t.records[1].entry == &g_components[1] && std::strcmp(g_components[0].definition, "PC_MENU_ENTRY_BRANCH_TEMPLATE") == 0
        && std::strcmp(g_components[1].definition, "PC_MENU_ENTRY_TEMPLATE") == 0 && g_components[1].cleared && !g_components[0].cleared
        && g_nActions == 14 && g_actions[2].type == 0x7d7 && g_actions[2].params[0] == &viewport && g_actions[2].params[2] == &owner && g_actions[0].params[1] == &owner && g_actions[6].longs[1] == 3
        && g_components[0].nSelected == 2 && g_components[0].nFocus == 3 && g_components[0].secondary == 0x7d8
        && t.nBottom == 1 && t.bottom[0] == &g_components[0] && t.nTop == 1 && t.top[0] == &g_components[1];
    if (!ok) { std::printf("0059f51e_TEST FAIL\n"); return 1; }
    std::printf("0059f51e_TEST PASS\n");
    return 0;
}