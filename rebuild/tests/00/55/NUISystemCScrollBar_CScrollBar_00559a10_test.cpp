// Behaviour test for NUISystem::CScrollBar::~CScrollBar @ 0x00559A10: the dragged-down action list is deleted before the
// dragged-up one, a null list is skipped, and the CDraggable base destructor runs last.
#include <stdio.h>
static int g_order[8]; static int g_n = 0;
class CActionListType { public: int Id; ~CActionListType() { g_order[g_n++] = Id; } };
namespace NUISystem {
class CDraggable { public: virtual ~CDraggable() { g_order[g_n++] = 99; } };
class CScrollBar : public CDraggable { public:
    CActionListType* PActionsOnDraggedUp;
    CActionListType* PActionsOnDraggedDown;
    unsigned int     AmountActionsUp;
    virtual ~CScrollBar();
};
CScrollBar::~CScrollBar()
{
    delete PActionsOnDraggedDown;
    delete PActionsOnDraggedUp;
}
}
static int fail(int line) { printf("00559a10_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    NUISystem::CScrollBar* bar = new NUISystem::CScrollBar;
    bar->PActionsOnDraggedUp = new CActionListType; bar->PActionsOnDraggedUp->Id = 1;
    bar->PActionsOnDraggedDown = new CActionListType; bar->PActionsOnDraggedDown->Id = 2;
    NUISystem::CDraggable* base = bar;
    delete base;
    if (g_n != 3 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 99) return fail(__LINE__);
    g_n = 0;
    NUISystem::CScrollBar* bar2 = new NUISystem::CScrollBar;
    bar2->PActionsOnDraggedUp = new CActionListType; bar2->PActionsOnDraggedUp->Id = 5;
    bar2->PActionsOnDraggedDown = 0;
    delete bar2;
    if (g_n != 2 || g_order[0] != 5 || g_order[1] != 99) return fail(__LINE__);
    printf("00559a10_TEST PASS\n");
    return 0;
}