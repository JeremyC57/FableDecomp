// Behaviour test for NUISystem::CScrollBar::CScrollBar (0x00559830): base built with the owner, list pointers and counters zeroed before GetDataFromDef runs.
#include <cstdio>
struct CActionList;
struct C2DVector { float X, Y; };
struct CDraggable {
    long m_owner;
    CDraggable(long owner) : m_owner(owner) {}
    virtual ~CDraggable() {}
};
static bool g_sawZeroes = false;
struct CScrollBar : CDraggable {
    CActionList* PActionsOnDraggedUp;
    CActionList* PActionsOnDraggedDown;
    C2DVector DeltaBeforeAction, DeltaForActionUp, DeltaForActionDown;
    unsigned long AmountActionsUp, AmountActionsDown, MaxAmountActionsUp, MaxAmountActionsDown;
    CScrollBar(long owner)
        : CDraggable(owner), PActionsOnDraggedUp(0), PActionsOnDraggedDown(0),
          AmountActionsUp(0), AmountActionsDown(0), MaxAmountActionsUp(0), MaxAmountActionsDown(0)
    {
        GetDataFromDef();
    }
    void GetDataFromDef()
    {
        g_sawZeroes = PActionsOnDraggedUp == 0 && PActionsOnDraggedDown == 0 && AmountActionsUp == 0 &&
                      AmountActionsDown == 0 && MaxAmountActionsUp == 0 && MaxAmountActionsDown == 0;
        MaxAmountActionsUp = 5;
    }
};
int main()
{
    CScrollBar bar(1234);
    if (bar.m_owner != 1234 || !g_sawZeroes || bar.MaxAmountActionsUp != 5) { std::printf("00559830_TEST FAIL\n"); return 1; }
    std::printf("00559830_TEST PASS\n");
    return 0;
}