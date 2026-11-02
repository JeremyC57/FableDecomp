// Behaviour test for 0x008B6A20 CCreatureAction_GoUnderground::FinishAction(): PerformAction runs only if the action never
// performed, an AI creature (class type 1) gets SuspendAI(false, 0), then the combat base FinishAction runs.
#include <cstdio>

static int g_combatFinish = 0;

class CThing {
public:
    unsigned char Type;
    unsigned long GetType() const { return Type; }
};
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase {
public:
    int SuspendCalls; bool LastSuspend; void* LastSuspender;
    void SuspendAI(bool suspend, void* suspender) { ++SuspendCalls; LastSuspend = suspend; LastSuspender = suspender; }
};

template <class T> class CIntelligentPointer {
public:
    CIntelligentPointer() : Item(0) { }
    T* Get() const { return Item; }
    T* operator->() const { return Get(); }
    void Set(T* t) { Item = t; }
private:
    T* Item;
};

class CCreatureActionBase {
public:
    CCreatureActionBase() : PerformedAction(false) { }
    virtual ~CCreatureActionBase() { }
    virtual void PerformAction() = 0;
    virtual void FinishAction() { }
    bool HasPerformedAction() const { return PerformedAction; }
    CIntelligentPointer<CThingCreatureBase> PCreature;
    bool PerformedAction;
};
class CCombatActionBase : public CCreatureActionBase {
protected:
    virtual void FinishAction() { ++g_combatFinish; }
};
class CCreatureAction_GoUnderground : public CCombatActionBase {
public:
    CCreatureAction_GoUnderground() : Performs(0) { }
    int Performs;
    virtual void PerformAction() { ++Performs; PerformedAction = true; }
    virtual void FinishAction();
};

void CCreatureAction_GoUnderground::FinishAction()
{
    if (!HasPerformedAction())
        PerformAction();
    if (PCreature->GetType() == 1) {
        CThingAICreature* creature = static_cast<CThingAICreature*>(PCreature.Get());
        creature->SuspendAI(false, 0);
    }
    CCombatActionBase::FinishAction();
}

int main()
{
    CThingAICreature ai; ai.Type = 1; ai.SuspendCalls = 0; ai.LastSuspend = true; ai.LastSuspender = &ai;
    CCreatureAction_GoUnderground a;
    a.PCreature.Set(&ai);
    a.FinishAction();
    if (a.Performs != 1 || ai.SuspendCalls != 1 || ai.LastSuspend || ai.LastSuspender != 0 || g_combatFinish != 1) { std::printf("008b6a20_TEST FAIL\n"); return 1; }
    a.FinishAction();                                         // already performed: no second PerformAction
    if (a.Performs != 1 || ai.SuspendCalls != 2 || g_combatFinish != 2) { std::printf("008b6a20_TEST FAIL\n"); return 1; }
    CThingAICreature hero; hero.Type = 2; hero.SuspendCalls = 0;
    CCreatureAction_GoUnderground b;
    b.PCreature.Set(&hero);
    b.FinishAction();                                         // not an AI creature: no SuspendAI
    if (b.Performs != 1 || hero.SuspendCalls != 0 || g_combatFinish != 3) { std::printf("008b6a20_TEST FAIL\n"); return 1; }
    std::printf("008b6a20_TEST PASS\n");
    return 0;
}