// Behaviour test for CCreatureAction_RiseUp::FinishAction (0x008B6A90): an unperformed action is performed first; an AI creature
// (CClass::Type 1) gets SuspendAI(false, 0); every path ends in the combat-action base FinishAction.
#include <cstdio>
class CBaseObject { public: virtual ~CBaseObject() { } };
class CClass : public CBaseObject { public: unsigned char Type; unsigned long GetType() const { return Type; } };
class CThing : public CClass { };
class CThingCreatureBase : public CThing { };
static int gSuspendCalls; static bool gSuspendArg; static void* gSuspender; static CThing* gSuspended;
class CThingAICreature : public CThingCreatureBase { public:
    void SuspendAI(bool suspend, void* suspender) { ++gSuspendCalls; gSuspendArg = suspend; gSuspender = suspender; gSuspended = this; }
};
static int gGetPItemCalls;
class CBaseIntelligentPointer { public:
    CBaseObject* Item;
    CBaseObject* GetPItem() const { ++gGetPItemCalls; return Item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
static int gOrder; static int gPerformAt, gBaseFinishAt, gSuspendAt;
class CCreatureActionBase : public CBaseObject { public:
    bool PerformedAction;
    virtual void PerformAction() { }
    virtual void FinishAction() { }
    bool HasPerformedAction() const { return PerformedAction; }
};
class CCombatActionBase : public CCreatureActionBase { public:
    CIntelligentPointer<CThingCreatureBase> PCreature;
protected:
    virtual void FinishAction() { gBaseFinishAt = ++gOrder; }
};
class CCreatureAction_RiseUp : public CCombatActionBase { public:
    virtual void PerformAction() { gPerformAt = ++gOrder; PerformedAction = true; }
    virtual void FinishAction();
};
void CCreatureAction_RiseUp::FinishAction()
{
    if (!HasPerformedAction())
        PerformAction();
    if (PCreature.GetPItem()->GetType() == 1) {
        CThingAICreature* creature = static_cast<CThingAICreature*>(PCreature.GetPItem());
        gSuspendAt = gOrder + 1;
        creature->SuspendAI(false, 0);
    }
    CCombatActionBase::FinishAction();
}
static void Reset() { gOrder = gPerformAt = gBaseFinishAt = gSuspendAt = 0; gSuspendCalls = 0; gGetPItemCalls = 0; gSuspended = 0; gSuspendArg = true; gSuspender = (void*)1; }
int main()
{
    CThingAICreature ai; ai.Type = 1;
    CThingCreatureBase hero; hero.Type = 2;
    CCreatureAction_RiseUp action; action.PCreature.Item = &ai; action.PerformedAction = false;
    CCreatureActionBase* base = &action;
    Reset(); base->FinishAction();
    if (gPerformAt != 1 || gSuspendCalls != 1 || gSuspended != &ai || gSuspendArg || gSuspender != 0 || gBaseFinishAt != 2
        || gGetPItemCalls != 2 || !action.PerformedAction) { std::printf("008b6a90_TEST FAIL\n"); return 1; }
    Reset(); base->FinishAction();                       // already performed: no second PerformAction
    if (gPerformAt != 0 || gSuspendCalls != 1 || gBaseFinishAt != 1) { std::printf("008b6a90_TEST FAIL\n"); return 1; }
    action.PCreature.Item = &hero; Reset(); base->FinishAction();   // not an AI creature: no SuspendAI
    if (gSuspendCalls != 0 || gBaseFinishAt != 1 || gGetPItemCalls != 1) { std::printf("008b6a90_TEST FAIL\n"); return 1; }
    std::printf("008b6a90_TEST PASS\n");
    return 0;
}