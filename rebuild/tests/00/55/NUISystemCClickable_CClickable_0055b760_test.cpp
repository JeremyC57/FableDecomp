// Behaviour test: NUISystem::CClickable::~CClickable (0x0055B760) -- the four owned action lists are deleted in retail's
// order (LeftClicked, RightClicked, LeftHeld, LeftUnclicked), null ones skipped, then the CHoverable base runs.
#include <cstdio>
static int g_order[8]; static int g_n = 0; static int g_hoverable = 0;
struct CActionList { int id; CActionList(int i) : id(i) { } ~CActionList() { g_order[g_n++] = id; } };
struct CHoverable { virtual ~CHoverable() { g_order[g_n++] = 100; ++g_hoverable; } };
struct CClickable : CHoverable {
    CActionList* PActionsOnLeftClicked; CActionList* PActionsOnLeftHeld; CActionList* PActionsOnLeftUnclicked;
    CActionList* PActionsOnRightClicked;
    virtual ~CClickable()
    {
        delete PActionsOnLeftClicked;
        delete PActionsOnRightClicked;
        delete PActionsOnLeftHeld;
        delete PActionsOnLeftUnclicked;
    }
};
int main()
{
    CClickable* c = new CClickable;
    c->PActionsOnLeftClicked = new CActionList(1); c->PActionsOnLeftHeld = new CActionList(2);
    c->PActionsOnLeftUnclicked = new CActionList(3); c->PActionsOnRightClicked = new CActionList(4);
    CHoverable* h = c;
    delete h;
    if (g_n != 5 || g_order[0] != 1 || g_order[1] != 4 || g_order[2] != 2 || g_order[3] != 3 || g_order[4] != 100)
    { std::printf("0055b760_TEST FAIL\n"); return 1; }
    CClickable* d = new CClickable;
    d->PActionsOnLeftClicked = 0; d->PActionsOnLeftHeld = new CActionList(5); d->PActionsOnLeftUnclicked = 0;
    d->PActionsOnRightClicked = 0;
    delete d;
    if (g_n != 7 || g_order[5] != 5 || g_hoverable != 2) { std::printf("0055b760_TEST FAIL\n"); return 1; }
    std::printf("0055b760_TEST PASS\n");
    return 0;
}