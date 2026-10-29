// Behaviour test for NUISystem::CScrollBarOutside::~CScrollBarOutside @ 0x00559520: the two click-above / click-under action
// lists are deleted (above first, then under), a null list is skipped, and the CClickable base destructor runs after them.
#include <stdio.h>
static int g_order[8]; static int g_n = 0;
class CActionListType { public: int Id; ~CActionListType() { g_order[g_n++] = Id; } };
namespace NUISystem {
class CClickable { public: virtual ~CClickable() { g_order[g_n++] = 99; } };
class CScrollBarOutside : public CClickable { public:
    void*            PScrollBar;
    CActionListType* PActionsOnLeftClickedAbove;
    CActionListType* PActionsOnLeftClickedUnder;
    virtual ~CScrollBarOutside();
};
CScrollBarOutside::~CScrollBarOutside()
{
    delete PActionsOnLeftClickedAbove;
    delete PActionsOnLeftClickedUnder;
}
}
static int fail(int line) { printf("00559520_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    NUISystem::CScrollBarOutside* bar = new NUISystem::CScrollBarOutside;
    bar->PActionsOnLeftClickedAbove = new CActionListType; bar->PActionsOnLeftClickedAbove->Id = 1;
    bar->PActionsOnLeftClickedUnder = new CActionListType; bar->PActionsOnLeftClickedUnder->Id = 2;
    NUISystem::CClickable* base = bar;
    delete base;
    if (g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 99) return fail(__LINE__);
    g_n = 0;
    NUISystem::CScrollBarOutside* bar2 = new NUISystem::CScrollBarOutside;
    bar2->PActionsOnLeftClickedAbove = 0;
    bar2->PActionsOnLeftClickedUnder = new CActionListType; bar2->PActionsOnLeftClickedUnder->Id = 7;
    delete bar2;
    if (g_n != 2 || g_order[0] != 7 || g_order[1] != 99) return fail(__LINE__);
    printf("00559520_TEST PASS\n");
    return 0;
}