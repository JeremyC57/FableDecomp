// Behaviour test for 006621c0: body copied from src/006621c0.cpp (CThingCreatureBase::IsFreeToFinishAction), the virtual callee stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCreatureActionBase { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {}
    virtual bool IsActionReadyForInterruption() { ++readyCalls; return ready; }
    unsigned char _pad04[0x1c];
    long InterruptionGroupID;
    long InterruptionPriority;
    unsigned char _pad28[0x39];
    bool Finished;
    bool ready; int readyCalls;
    CCreatureActionBase() : InterruptionGroupID(0), InterruptionPriority(0), Finished(false), ready(false), readyCalls(0) {}
};
class CThingCreatureBase { public:
    unsigned char _pad000[0x100];
    CCreatureActionBase* PCurrentAction;
    bool IsPerformingAnyAction()
    {
        if (PCurrentAction && !PCurrentAction->Finished)
            return true;
        return false;
    }
    bool IsFreeToFinishAction();
};
bool CThingCreatureBase::IsFreeToFinishAction()
{
    CCreatureActionBase* action = PCurrentAction;
    if (!(action && action->InterruptionGroupID == 100 && action->InterruptionPriority >= 250)) {
        if (!IsPerformingAnyAction() || action->IsActionReadyForInterruption() || PCurrentAction->InterruptionPriority < 50)
            return true;
    }
    return false;
}
int main()
{
    CThingCreatureBase c; CCreatureActionBase a;
    // 1. no action: free
    c.PCurrentAction = 0; CHECK(c.IsFreeToFinishAction() == true);
    // 2. a finished action: free (not performing any action)
    c.PCurrentAction = &a; a.Finished = true; CHECK(c.IsFreeToFinishAction() == true); CHECK(a.readyCalls == 0);
    // 3. running, ready for interruption: free
    a.Finished = false; a.ready = true; CHECK(c.IsFreeToFinishAction() == true); CHECK(a.readyCalls == 1);
    // 4. running, not ready, low priority: free; high priority: not free
    a.ready = false; a.InterruptionPriority = 10; CHECK(c.IsFreeToFinishAction() == true);
    a.InterruptionPriority = 50; CHECK(c.IsFreeToFinishAction() == false);
    // 5. uninterruptible top-priority group-100 action: never free
    a.InterruptionGroupID = 100; a.InterruptionPriority = 250; a.Finished = true; a.ready = true; CHECK(c.IsFreeToFinishAction() == false);
    a.InterruptionPriority = 249; CHECK(c.IsFreeToFinishAction() == true);
    if (fails == 0) printf("006621c0_TEST PASS\n"); else printf("006621c0_TEST FAIL\n");
    return fails;
}