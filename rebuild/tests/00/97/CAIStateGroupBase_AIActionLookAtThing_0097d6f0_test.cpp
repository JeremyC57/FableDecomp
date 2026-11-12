// Behaviour test for 0x0097D6F0 CAIStateGroupBase::AIAction_LookAtThing: the same logic through a small copy of the model.
#include <cstdio>
enum EAIActionReturn {AI_ACTION_NULL=0,AI_ACTION_TERMINATE_PROCESS=1,AI_ACTION_FINISHED=2,AI_ACTION_FAILED=3,AI_ACTION_IN_PROGRESS=4};
enum EAIActionRepeat {ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2};
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CTCGraphicAppearance {
    C3DVector Focal; mutable float LastTime;
    C3DVector GetFocalPos(float time) const { LastTime = time; return Focal; }
};
class CThing { public:
    CTCGraphicAppearance* PTCGraphicAppearance; C3DVector Pos; bool Dead;
    CThing() : PTCGraphicAppearance(0), Pos(0, 0, 0), Dead(false) {}
    bool IsDead() const { return Dead; }
    const CTCGraphicAppearance* GetTCGraphicAppearance() const { return PTCGraphicAppearance; }
    const C3DVector& GetPos() const { return Pos; }
};
class CThingAICreature : public CThing { public:
    int InViewAfter;      // IsPosInViewAngle answers true from this call number on (0 = never)
    int Calls; C3DVector LastPos; float LastAngle; bool LastA, LastB;
    CThingAICreature() : InViewAfter(0), Calls(0), LastPos(0, 0, 0), LastAngle(0), LastA(false), LastB(false) {}
    bool IsPosInViewAngle(const C3DVector& pos, float angle, bool a, bool b)
    {
        ++Calls; LastPos = pos; LastAngle = angle; LastA = a; LastB = b;
        return InViewAfter != 0 && Calls >= InViewAfter;
    }
};
struct CIntelligentPointer {
    const CThing* P;
    const CThing* Get() const { return P; }
    const CThing* operator->() const { return P; }
};
static bool IS_THING_ALIVE(const CIntelligentPointer& thing)
{
    if (thing.Get() != 0) {
        if (!thing->IsDead()) return true;
    }
    return false;
}
class CBrainOutput { public:
    unsigned long Types; long LookFlags; const CThing* LookAtThing;
    CBrainOutput() : Types(0), LookFlags(0), LookAtThing(0) {}
    void SetAsLookAtThing(const CThing& thing, long lookFlags) { Types |= 4; LookAtThing = &thing; LookFlags = lookFlags; }
};
class CAIBrain { public:
    CThingAICreature* Creature; int Outputs; int Suspends; CBrainOutput Last;
    CAIBrain() : Creature(0), Outputs(0), Suspends(0) {}
    CThingAICreature& GetCreature() const { return *Creature; }
    void SetOutput(const CBrainOutput& output) { Last = output; ++Outputs; }
    bool SuspendSuspendableProcess() { ++Suspends; return true; }
};
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    int StopAtCall;       // IsStopping answers true from this call number on (0 = never)
    int StopCalls;
    CAIStateGroupBase() : PBrain(0), StopAtCall(0), StopCalls(0) {}
    bool IsStopping() { ++StopCalls; return StopAtCall != 0 && StopCalls >= StopAtCall; }
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer thing, long lookFlags, float viewAngle);
};
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer thing, long lookFlags, float viewAngle)
{
    if (IsStopping())
        return AI_ACTION_TERMINATE_PROCESS;
    EAIActionReturn result;
    do {
        if (IS_THING_ALIVE(thing)) {
            C3DVector pos;
            if (thing->GetTCGraphicAppearance())
                pos = thing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
            else
                pos = thing->GetPos();
            if (thing.Get() == &GetCreature() || GetCreature().IsPosInViewAngle(pos, viewAngle, (lookFlags & 2) > 0, (lookFlags & 4) > 0))
                result = AI_ACTION_FINISHED;
            else {
                CBrainOutput output;
                output.SetAsLookAtThing(*thing.Get(), lookFlags);
                PBrain->SetOutput(output);
                result = AI_ACTION_IN_PROGRESS;
            }
        } else
            result = AI_ACTION_FAILED;
        PBrain->SuspendSuspendableProcess();
        if (IsStopping())
            return AI_ACTION_TERMINATE_PROCESS;
    } while (repeat == ACTION_REPEAT_UNTIL_FINISHED && result == AI_ACTION_IN_PROGRESS);
    return result;
}
struct Rig {
    CThingAICreature creature; CAIBrain brain; CAIStateGroupBase group; CThing thing; CTCGraphicAppearance look;
    Rig() { brain.Creature = &creature; group.PBrain = &brain; thing.Pos = C3DVector(1, 2, 3); look.Focal = C3DVector(7, 8, 9); look.LastTime = 0; }
    EAIActionReturn Run(EAIActionRepeat repeat, const CThing* target, long flags, float angle)
    { CIntelligentPointer p; p.P = target; return group.AIAction_LookAtThing(repeat, p, flags, angle); }
};
static int Check()
{
    { Rig r; r.group.StopAtCall = 1;                                   // stopping on entry: nothing else happens
      if (r.Run(ACTION_REPEAT_CALLBACK, &r.thing, 0, 1.0f) != AI_ACTION_TERMINATE_PROCESS || r.brain.Suspends != 0 || r.creature.Calls != 0) return 1; }
    { Rig r;                                                           // null thing: failed, one suspend, no output
      if (r.Run(ACTION_REPEAT_UNTIL_FINISHED, 0, 0, 1.0f) != AI_ACTION_FAILED || r.brain.Suspends != 1 || r.brain.Outputs != 0) return 2; }
    { Rig r; r.thing.Dead = true;                                      // dead thing: failed
      if (r.Run(ACTION_REPEAT_UNTIL_FINISHED, &r.thing, 0, 1.0f) != AI_ACTION_FAILED || r.creature.Calls != 0 || r.brain.Suspends != 1) return 3; }
    { Rig r;                                                           // the thing is the creature itself: finished without a view test
      if (r.Run(ACTION_REPEAT_UNTIL_FINISHED, &r.creature, 6, 1.0f) != AI_ACTION_FINISHED || r.creature.Calls != 0 || r.brain.Outputs != 0) return 4; }
    { Rig r; r.creature.InViewAfter = 1;                               // in view at once: position from GetPos, flag 2 only
      if (r.Run(ACTION_REPEAT_UNTIL_FINISHED, &r.thing, 2, 0.5f) != AI_ACTION_FINISHED) return 5;
      if (r.creature.LastPos.X != 1 || r.creature.LastPos.Y != 2 || r.creature.LastPos.Z != 3) return 6;
      if (r.creature.LastAngle != 0.5f || !r.creature.LastA || r.creature.LastB || r.brain.Outputs != 0 || r.brain.Suspends != 1) return 7; }
    { Rig r; r.thing.PTCGraphicAppearance = &r.look;                   // not in view, no repeat: focal point at time 1, output posted
      if (r.Run(ACTION_REPEAT_CALLBACK, &r.thing, 0x25, 0.25f) != AI_ACTION_IN_PROGRESS) return 8;
      if (r.creature.LastPos.X != 7 || r.creature.LastPos.Y != 8 || r.creature.LastPos.Z != 9 || r.look.LastTime != 1.0f) return 9;
      if (r.creature.LastA || !r.creature.LastB) return 10;
      if (r.brain.Outputs != 1 || r.brain.Last.Types != 4 || r.brain.Last.LookAtThing != &r.thing || r.brain.Last.LookFlags != 0x25) return 11;
      if (r.brain.Suspends != 1 || r.group.StopCalls != 2) return 12; }
    { Rig r; r.creature.InViewAfter = 4;                               // repeat: three passes in progress, the fourth finishes
      if (r.Run(ACTION_REPEAT_UNTIL_FINISHED, &r.thing, 6, 1.0f) != AI_ACTION_FINISHED) return 13;
      if (r.creature.Calls != 4 || r.brain.Outputs != 3 || r.brain.Suspends != 4 || r.group.StopCalls != 5) return 14;
      if (!r.creature.LastA || !r.creature.LastB) return 15; }
    { Rig r; r.group.StopAtCall = 3;                                   // told to stop after the second suspend
      if (r.Run(ACTION_REPEAT_UNTIL_FINISHED, &r.thing, 0, 1.0f) != AI_ACTION_TERMINATE_PROCESS || r.brain.Outputs != 2 || r.brain.Suspends != 2) return 16; }
    { Rig r;                                                           // the thing dies between two calls: failed on the next one
      if (r.Run(ACTION_REPEAT_CALLBACK, &r.thing, 0, 1.0f) != AI_ACTION_IN_PROGRESS) return 17;
      r.thing.Dead = true;
      if (r.Run(ACTION_REPEAT_UNTIL_FINISHED, &r.thing, 0, 1.0f) != AI_ACTION_FAILED || r.brain.Outputs != 1) return 18; }
    return 0;
}
int main()
{
    int bad = Check();
    if (bad) { std::printf("0097d6f0_TEST FAIL\n"); return 1; }
    std::printf("0097d6f0_TEST PASS\n");
    return 0;
}