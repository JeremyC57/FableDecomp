// Behaviour test for CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) (0x0062EBC0): the copy
// detaches the base object record, re-attaches both intelligent pointers to the source items, copies the plain
// fields, add-refs both counted animation entries and copy-constructs the containers.
#include <cstdio>

class CThing { public: int id; };
class CBaseObject { public:
    void* IntelligentPointer;
    CBaseObject() : IntelligentPointer((void*)1) {}
    CBaseObject(const CBaseObject&) : IntelligentPointer(0) {}
    virtual ~CBaseObject() {}
};
static int g_setCalls = 0;
struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; ++g_setCalls; }
};
struct CCreaturePointer : CBaseIntelligentPointer {
    CCreaturePointer() {}
    CCreaturePointer(const CCreaturePointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingPointer : CBaseIntelligentPointer {
    CThingPointer() {}
    CThingPointer(const CThingPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
class CCPPointerInfo { public: long Count; };
class CAnimationEntry;
struct CAnimationEntryPointer {
    CAnimationEntry* Data; CCPPointerInfo* Info;
    CAnimationEntryPointer() : Data(0), Info(0) {}
    CAnimationEntryPointer(const CAnimationEntryPointer& other) : Data(other.Data), Info(other.Info)
    { if (Info) ++Info->Count; }
};
class CSoundPair { public: long FirstSound, LastSound; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
static int g_containerCopies = 0;
struct CContainer {
    int Size;
    CContainer() : Size(0) {}
    CContainer(const CContainer& o) : Size(o.Size) { ++g_containerCopies; }
};
class CCreatureActionBase : public CBaseObject { public:
    CCreaturePointer PCreature;
    long TotalTime, ActionTime, AnimDelayTime, FramesRunning, InterruptionGroupID, InterruptionPriority, ActionID;
    CThingPointer PItemRemoved;
    CSoundPair Sounds;
    unsigned long PlayingSoundID;
    float RunningTimeSeconds, LastRunningTimeSeconds;
    long StartFrame, PauseFrame, PauseTimer;
    C3DVector AnimMovementStep;
    bool AddedActionAnim, Finished, PerformedAction, Initialised;
    long NextMovementPosWorldFrame;
    C3DVector NextMovementPos;
    CAnimationEntryPointer PAnimEntry, PPreceedingPAnimEntry;
    CContainer ActionEvents, CutSceneEvents, DependentThings;
    CCreatureActionBase() {}
    CCreatureActionBase(const CCreatureActionBase& other);
};

CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase& other)
    : CBaseObject(other)
    , PCreature(other.PCreature)
    , TotalTime(other.TotalTime)
    , ActionTime(other.ActionTime)
    , AnimDelayTime(other.AnimDelayTime)
    , FramesRunning(other.FramesRunning)
    , InterruptionGroupID(other.InterruptionGroupID)
    , InterruptionPriority(other.InterruptionPriority)
    , ActionID(other.ActionID)
    , PItemRemoved(other.PItemRemoved)
    , Sounds(other.Sounds)
    , PlayingSoundID(other.PlayingSoundID)
    , RunningTimeSeconds(other.RunningTimeSeconds)
    , LastRunningTimeSeconds(other.LastRunningTimeSeconds)
    , StartFrame(other.StartFrame)
    , PauseFrame(other.PauseFrame)
    , PauseTimer(other.PauseTimer)
    , AnimMovementStep(other.AnimMovementStep)
    , AddedActionAnim(other.AddedActionAnim)
    , Finished(other.Finished)
    , PerformedAction(other.PerformedAction)
    , Initialised(other.Initialised)
    , NextMovementPosWorldFrame(other.NextMovementPosWorldFrame)
    , NextMovementPos(other.NextMovementPos)
    , PAnimEntry(other.PAnimEntry)
    , PPreceedingPAnimEntry(other.PPreceedingPAnimEntry)
    , ActionEvents(other.ActionEvents)
    , CutSceneEvents(other.CutSceneEvents)
    , DependentThings(other.DependentThings)
{
}

int main()
{
    CThing creature, item;
    CCPPointerInfo info1 = { 1 }, info2 = { 5 };
    CCreatureActionBase a;
    a.PCreature.PItem = &creature; a.PItemRemoved.PItem = &item;
    a.TotalTime = 10; a.ActionID = 42; a.Sounds.FirstSound = 3; a.Sounds.LastSound = 4; a.RunningTimeSeconds = 1.5f;
    a.AnimMovementStep.X = 1.0f; a.AnimMovementStep.Y = 2.0f; a.AnimMovementStep.Z = 3.0f; a.Finished = true; a.Initialised = false;
    a.NextMovementPos.X = 4.0f; a.NextMovementPos.Y = 5.0f; a.NextMovementPos.Z = 6.0f;
    a.PAnimEntry.Info = &info1; a.PPreceedingPAnimEntry.Info = &info2; a.ActionEvents.Size = 7;
    CCreatureActionBase b(a);
    bool ok = b.IntelligentPointer == 0 && b.PCreature.PItem == &creature && b.PItemRemoved.PItem == &item
        && g_setCalls == 2 && b.TotalTime == 10 && b.ActionID == 42 && b.Sounds.LastSound == 4
        && b.RunningTimeSeconds == 1.5f && b.AnimMovementStep.Z == 3.0f && b.Finished && !b.Initialised
        && b.NextMovementPos.Y == 5.0f && b.PAnimEntry.Info == &info1 && info1.Count == 2 && info2.Count == 6
        && b.ActionEvents.Size == 7 && g_containerCopies == 3;
    if (!ok) { std::printf("0062ebc0_TEST FAIL\n"); return 1; }
    std::printf("0062ebc0_TEST PASS\n");
    return 0;
}