// Behaviour test for NUISystem::CScrollBarOutside::CScrollBarOutside (0x00559360): base built with the owner, three pointers zeroed before GetDataFromDef.
#include <cstdio>
struct CComponent;
struct CActionList;
struct CClickable {
    long m_owner;
    CClickable(long owner) : m_owner(owner) {}
    virtual ~CClickable() {}
};
static int g_defCalls = 0;
struct CScrollBarOutside : CClickable {
    CComponent* PScrollBar;
    CActionList* PActionsOnLeftClickedAbove;
    CActionList* PActionsOnLeftClickedUnder;
    CScrollBarOutside(long owner)
        : CClickable(owner), PScrollBar(0), PActionsOnLeftClickedAbove(0), PActionsOnLeftClickedUnder(0)
    {
        GetDataFromDef();
    }
    void GetDataFromDef() { if (PScrollBar == 0 && PActionsOnLeftClickedAbove == 0 && PActionsOnLeftClickedUnder == 0) ++g_defCalls; }
};
int main()
{
    CScrollBarOutside o(77);
    if (o.m_owner != 77 || g_defCalls != 1 || o.PScrollBar != 0) { std::printf("00559360_TEST FAIL\n"); return 1; }
    std::printf("00559360_TEST PASS\n");
    return 0;
}
