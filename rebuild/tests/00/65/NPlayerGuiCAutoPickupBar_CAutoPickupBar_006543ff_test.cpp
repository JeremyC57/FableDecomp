#include <cstdio>
struct CStatBarInfo { int kind; };
struct CStatBase {
    CStatBarInfo* m_info;
    CStatBase(CStatBarInfo* info) : m_info(info) { }
    virtual ~CStatBase() { }
};
struct CRetailList_AutoPickup { void* Node; CRetailList_AutoPickup() : Node(&Node) { } };
struct CAutoPickupBar : CStatBase {
    CRetailList_AutoPickup Pickups; long CurrentDefIndex, CurrentCount; float CurrentTimeLeft, WaitingTimeLeft;
    CAutoPickupBar(CStatBarInfo* info) : CStatBase(info), Pickups(), CurrentDefIndex(0), CurrentCount(0), CurrentTimeLeft(0.0f), WaitingTimeLeft(0.0f) { }
};
int main()
{
    CStatBarInfo i; i.kind = 7;
    CAutoPickupBar b(&i);
    if (b.m_info != &i || b.Pickups.Node != &b.Pickups.Node || b.CurrentDefIndex || b.CurrentCount || b.CurrentTimeLeft != 0.0f || b.WaitingTimeLeft != 0.0f) { std::printf("006543ff_TEST FAIL\n"); return 1; }
    std::printf("006543ff_TEST PASS\n");
    return 0;
}