// Behaviour test for CAIStateGroupBase::AIAction_PerformActionTargetFaceAndWait @ 0x0097DCB0: post once, face while performing.
#include <cstdio>
struct CThing { unsigned char m_flags91; };
struct CIntelligentPointerConstThing {
    CThing* item;
    CIntelligentPointerConstThing(CThing* t) : item(t) { }
    CThing* GetPItem() const { return item; }
};
inline bool IS_THING_ALIVE(const CIntelligentPointerConstThing& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {
    long id;
    long GetActionID() const { return id; }
    const CCreatureActionBase* Clone() const { return this; }
};
struct CBrainOutput {
    unsigned long Types; long LookFlags; const CCreatureActionBase* PAction; CThing* LookAtThing;
    CBrainOutput() : Types(0), LookFlags(0), PAction(0), LookAtThing(0) { }
    void SetAsLookAtThing(CThing* thing, long flags) { Types |= 4; LookAtThing = thing; LookFlags = flags; }
    void SetAsPerformAction(const CCreatureActionBase* action) { Types |= 8; PAction = action->Clone(); }
};
struct CThingCreatureBase {
    const CCreatureActionBase* CurrentAction; int busyTicks;
    bool IsPerformingAnyAction() { return busyTicks > 0; }
};
struct CAIBrain {
    CThingCreatureBase* Creature; int posts, yields, looks, stopAfter; bool stopping; CThing* killAt; int killTick;
    CBrainOutput last;
    void SetBrainOutput(const CBrainOutput& o) { ++posts; last = o; if (o.Types & 4) ++looks; }
    void Yield()
    {
        ++yields;
        if (Creature->busyTicks > 0) --Creature->busyTicks;
        if (stopAfter && yields >= stopAfter) stopping = true;
        if (killAt && yields == killTick) killAt->m_flags91 = 1;
    }
};
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2 };
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    bool IsStopping() { return PBrain->stopping; }
    EAIActionReturn AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase& action,
                                                            CIntelligentPointerConstThing target, bool stopWhenReplaced);
};
EAIActionReturn CAIStateGroupBase::AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase& action,
                                                                         CIntelligentPointerConstThing target,
                                                                         bool stopWhenReplaced)
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    long id = action.GetActionID();
    CBrainOutput output;
    output.SetAsPerformAction(&action);
    PBrain->SetBrainOutput(output);
    PBrain->Yield();
    while (PBrain->Creature->IsPerformingAnyAction() && !IsStopping() && IS_THING_ALIVE(target)
           && (!stopWhenReplaced || id == PBrain->Creature->CurrentAction->GetActionID())) {
        CBrainOutput look;
        look.SetAsLookAtThing(target.GetPItem(), 0x11);
        PBrain->SetBrainOutput(look);
        PBrain->Yield();
    }
    if (IsStopping())
        return AI_ACTION_STOPPING;
    return AI_ACTION_DONE;
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }
static void Reset(CAIBrain& b, CThingCreatureBase& c, int busy)
{
    b.posts = b.yields = b.looks = b.stopAfter = 0; b.stopping = false; b.killAt = 0; b.killTick = 0; c.busyTicks = busy;
}

int main()
{
    CThing target; target.m_flags91 = 0;
    CCreatureActionBase action; action.id = 7;
    CCreatureActionBase other; other.id = 9;
    CThingCreatureBase creature; creature.CurrentAction = &action;
    CAIBrain brain; brain.Creature = &creature;
    CAIStateGroupBase group; group.PBrain = &brain;

    // Busy for 3 ticks: one action post, then two face-target looks with flags 0x11.
    Reset(brain, creature, 3);
    Check(group.AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointerConstThing(&target), true) == AI_ACTION_DONE, "done");
    Check(brain.posts == 3 && brain.looks == 2 && brain.yields == 3, "post then looks");
    Check(brain.last.LookFlags == 0x11 && brain.last.LookAtThing == &target, "look flags");

    // Stopping already: nothing posted.
    Reset(brain, creature, 3); brain.stopping = true;
    Check(group.AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointerConstThing(&target), true) == AI_ACTION_STOPPING, "stopping");
    Check(brain.posts == 0, "nothing posted when stopping");

    // Replaced action with stopWhenReplaced: no looks; without it: looks continue.
    Reset(brain, creature, 3); creature.CurrentAction = &other;
    group.AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointerConstThing(&target), true);
    Check(brain.looks == 0, "replaced stops waiting");
    Reset(brain, creature, 3);
    group.AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointerConstThing(&target), false);
    Check(brain.looks == 2, "replacement ignored");
    creature.CurrentAction = &action;

    // Target dies after the first look.
    Reset(brain, creature, 5); brain.killAt = &target; brain.killTick = 2;
    Check(group.AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointerConstThing(&target), true) == AI_ACTION_DONE, "done on death");
    Check(brain.looks == 1, "dead target ends facing");
    target.m_flags91 = 0;

    // Stopped mid-wait.
    Reset(brain, creature, 5); brain.stopAfter = 2;
    Check(group.AIAction_PerformActionTargetFaceAndWait(action, CIntelligentPointerConstThing(&target), true) == AI_ACTION_STOPPING, "stopped mid-wait");
    Check(brain.looks == 1, "one look before stop");

    if (failures == 0)
        std::printf("0097dcb0_TEST PASS\n");
    return failures;
}
