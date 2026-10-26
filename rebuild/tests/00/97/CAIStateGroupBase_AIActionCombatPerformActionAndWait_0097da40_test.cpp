// Behaviour test for 0x0097DA40 CAIStateGroupBase::AIAction_CombatPerformActionAndWait (model copy over stub brain/creature).
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;
static int g_released = 0;
struct CBaseIntelligentPointer { CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) {} ~CBaseIntelligentPointer() {} };
struct CCountBlock { long Count; void (__fastcall* Destroy)(void* object); void* Object; };
static void __fastcall DestroyAction(void*) { ++g_released; }
struct CCountedPointer {
    void* Object; CCountBlock* Block;
    CCountedPointer() : Object(0), Block(0) {}
    void Reset(void* object) { Object = object; Block = new CCountBlock; Block->Count = 1; Block->Destroy = DestroyAction; Block->Object = object; }
    ~CCountedPointer() { if (Block) { --Block->Count; if (Block->Count == 0) { Block->Destroy(Block->Object); delete Block; } } Object = 0; Block = 0; }
};
class CBrainOutput { public:
    unsigned long Flags; CCountedPointer PAction; CBaseIntelligentPointer PTarget;
    CBrainOutput() : Flags(0) {}
};
class CCreatureActionBase { public:
    long ID;
    void* Clone() const { return (void*)this; }
    long GetActionID() const { return ID; }
};
static int g_yields = 0; static int g_freeAfter = 0; static int g_performYields = 0; static int g_stopAfter = 1000;
static int g_stoppingCalls = 0; static unsigned long g_postedFlags = 0; static void* g_postedAction = 0;
static CCreatureActionBase* g_replaceWith = 0; static int g_replaceAt = -1;
class CThingCreatureBase { public:
    CCreatureActionBase* CurrentAction;
    bool IsFreeToPerformAction(const CCreatureActionBase*) { return g_yields >= g_freeAfter; }
    bool IsPerformingAnyAction() { return CurrentAction != 0; }
};
class CAIBrain { public:
    CThing* Creature;
    void Yield();
    void SetBrainOutput(const CBrainOutput& output) { g_postedFlags = output.Flags; g_postedAction = output.PAction.Object; }
};


class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    bool IsStopping() { return ++g_stoppingCalls > g_stopAfter; }
    long Run(CCreatureActionBase* action, bool stop);
};
static CThingCreatureBase g_creature;
static int g_postYield = -1;
void CAIBrain::Yield()
{
    ++g_yields;
    if (g_postedAction && g_postYield < 0) g_postYield = g_yields;
    if (g_postYield >= 0 && g_yields - g_postYield == g_replaceAt) g_creature.CurrentAction = g_replaceWith;
    if (g_postYield >= 0 && g_yields - g_postYield >= g_performYields) g_creature.CurrentAction = 0;
}
static void Reset(CCreatureActionBase* current) { g_yields = 0; g_postYield = -1; g_stoppingCalls = 0; g_postedAction = 0; g_postedFlags = 0; g_creature.CurrentAction = current; }

long CAIStateGroupBase::Run(CCreatureActionBase* action, bool stopWhenReplaced)
{
    if (IsStopping())
        return 1;
    long id = action->GetActionID();
    CBrainOutput output;
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
    PBrain->SetBrainOutput(output);
    PBrain->Yield();
    while (((CThingCreatureBase*)PBrain->Creature)->IsPerformingAnyAction() && !IsStopping() &&
           (!stopWhenReplaced || id == ((CThingCreatureBase*)PBrain->Creature)->CurrentAction->GetActionID()))
        PBrain->Yield();
    if (IsStopping())
        return 1;
    return 2;
}


static int fail(int n) { std::printf("%s_TEST FAIL (%d)\n", "0097da40", n); return 1; }
int main()
{
    CAIBrain brain; brain.Creature = (CThing*)&g_creature;
    CAIStateGroupBase group; group.PBrain = &brain;
    CCreatureActionBase action; action.ID = 5; CCreatureActionBase other; other.ID = 9;
    Reset(&action); g_performYields = 3; g_freeAfter = 0;
    if (group.Run(&action, false) != 2 || g_postedFlags != 8 || g_postedAction != &action) return fail(1);
    if (g_yields != 4 || g_released != 1) return fail(2);   // one yield after posting, three while performing
    Reset(&action); g_performYields = 10; g_replaceWith = &other; g_replaceAt = 2;
    if (group.Run(&action, true) != 2 || g_yields != 3 || g_released != 2) return fail(3);   // replaced: stop waiting
    g_replaceAt = -1;
    Reset(&action); g_stopAfter = 0;
    if (group.Run(&action, false) != 1 || g_postedAction != 0) return fail(4);   // stopping at once
    g_stopAfter = 1000;
    std::printf("%s_TEST PASS\n", "0097da40");
    return 0;
}
