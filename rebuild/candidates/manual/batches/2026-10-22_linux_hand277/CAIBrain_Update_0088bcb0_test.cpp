// Behaviour test for CAIBrain::Update @ 0x0088BCB0: update period gating, pending group switch, output copy.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#define __forceinline inline
#endif
struct CCharString {
    const char* text;
    CCharString(const char* t = "") : text(t) { }
    CCharString(const CCharString& o) : text(o.text) { }
};
struct CBrainOutput {
    int value;
    int merges;
    CBrainOutput() : value(0), merges(0) { }
    void Merge(const CBrainOutput& other) { value = other.value; ++merges; }
};
struct CMainGameComponent {
    unsigned long frame;
    unsigned long GetWorldFrame() const { return frame; }
};
struct CThingAICreature {
    u64 UpdateOffset;
    unsigned char CreatureFlags;
    bool ready;
    bool IsReadyForBrain() const { return ready; }
};
struct CBrainUpdateZone {
    unsigned long UpdatePeriod;
    unsigned long PriorityCheckPeriod;
};
struct CAIStateGroupBase {
    CCharString Name;
    bool Terminated;
    unsigned char Priority;
    bool runnable, everyFrame;
    int starts, stops, resets;
    CAIStateGroupBase() : Terminated(false), Priority(0), runnable(true), everyFrame(false), starts(0), stops(0), resets(0) { }
    void Start() { ++starts; }
    bool IsRunnable() { return runnable; }
    bool UpdateEveryFrame() { return everyFrame; }
    void Terminate() { Terminated = true; }
    void Reset() { ++resets; Terminated = false; }
    void Stop() { ++stops; }
};
struct CAIBrain {
    CMainGameComponent* Component;
    CThingAICreature* Creature;
    int groupCount;
    CAIStateGroupBase* PActiveStateGroup;
    CAIStateGroupBase* PLastStateGroup;
    CAIStateGroupBase* PPendingStateGroup;
    CBrainOutput Output;
    bool OutputValid, UpdatingStateGroup, StateGroupUpdated, InitialisedCreaturePosition;
    const CBrainUpdateZone* PCurrentUpdateZone;
    CBrainUpdateZone zone;
    CAIStateGroupBase* candidate;
    int processes, inits;
    bool produceOutput;
    const CBrainUpdateZone* GetUpdateZone(long distance) const { return distance < 100 ? &zone : 0; }
    void InitCreaturePosition() { ++inits; }
    CAIStateGroupBase* FindNextStateGroupToUse(long priority) { return (candidate && (priority < 0 || candidate->Priority > priority)) ? candidate : 0; }
    void SuspendableProcess() { ++processes; if (produceOutput) { OutputValid = true; Output.value = 42; } }
    __forceinline bool IsFrameDue(unsigned long period) const
    {
        u64 offset = Creature->UpdateOffset;
        return (offset + Component->GetWorldFrame()) % period == 0;
    }
    bool Update(long distance, CBrainOutput& output);
};
bool CAIBrain::Update(long distance, CBrainOutput& output)
{
    PCurrentUpdateZone = GetUpdateZone(distance);
    unsigned long updatePeriod = 1;
    unsigned long priorityPeriod = 1;
    bool outputCopied = false;
    OutputValid = false;
    StateGroupUpdated = false;
    if (PCurrentUpdateZone) {
        updatePeriod = PCurrentUpdateZone->UpdatePeriod;
        priorityPeriod = PCurrentUpdateZone->PriorityCheckPeriod;
    }
    if (!InitialisedCreaturePosition && Creature->IsReadyForBrain()) {
        InitCreaturePosition();
        InitialisedCreaturePosition = true;
    }
    if (!IsFrameDue(updatePeriod)) {
        if (!PActiveStateGroup || !PActiveStateGroup->UpdateEveryFrame())
            return false;
    }
    if (groupCount == 0)
        return false;
    bool stillRunnable = true;
    if (PActiveStateGroup && !PActiveStateGroup->IsRunnable()) {
        PActiveStateGroup->Terminate();
        stillRunnable = false;
    }
    if (PActiveStateGroup && !PActiveStateGroup->Terminated && IsFrameDue(priorityPeriod)) {
        CAIStateGroupBase* next = FindNextStateGroupToUse(PActiveStateGroup->Priority);
        if (next)
            PPendingStateGroup = next;
    }
    if (stillRunnable && !PPendingStateGroup && (!PActiveStateGroup || PActiveStateGroup->Terminated))
        PPendingStateGroup = FindNextStateGroupToUse(-1);
    if (PPendingStateGroup) {
        if (UpdatingStateGroup) {
            if (PActiveStateGroup && !PActiveStateGroup->Terminated)
                PActiveStateGroup->Terminate();
        } else {
            if (PActiveStateGroup) {
                PActiveStateGroup->Stop();
                if (OutputValid) {
                    outputCopied = OutputValid;
                    output.Merge(Output);
                }
                OutputValid = false;
            }
            PPendingStateGroup->Reset();
            PLastStateGroup = PActiveStateGroup;
            PActiveStateGroup = PPendingStateGroup;
            PPendingStateGroup = 0;
            PActiveStateGroup->Start();
            if (outputCopied)
                return true;
        }
    }
    if (!PActiveStateGroup || (Creature->CreatureFlags & 2))
        return false;
    {
    CCharString name(PActiveStateGroup->Name);
    SuspendableProcess();
    StateGroupUpdated = true;
    if (OutputValid) {
        output.Merge(Output);
        return true;
    }
    if (!UpdatingStateGroup && PActiveStateGroup->Terminated) {
        bool copied = false;
        PActiveStateGroup->Stop();
        if (OutputValid) {
            copied = OutputValid;
            output.Merge(Output);
        }
        OutputValid = false;
        PLastStateGroup = PActiveStateGroup;
        PActiveStateGroup = 0;
        if (copied)
            return true;
    }
    }
    return false;
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }

int main()
{
    CMainGameComponent component; component.frame = 3;
    CThingAICreature creature; creature.UpdateOffset = 0; creature.CreatureFlags = 0; creature.ready = true;
    CAIStateGroupBase a; a.Name = CCharString("A"); a.Priority = 5;
    CAIBrain brain;
    brain.Component = &component; brain.Creature = &creature; brain.groupCount = 1;
    brain.PActiveStateGroup = 0; brain.PLastStateGroup = 0; brain.PPendingStateGroup = 0;
    brain.OutputValid = false; brain.UpdatingStateGroup = false; brain.StateGroupUpdated = false;
    brain.InitialisedCreaturePosition = false; brain.PCurrentUpdateZone = 0;
    brain.zone.UpdatePeriod = 2; brain.zone.PriorityCheckPeriod = 1;
    brain.candidate = &a; brain.processes = 0; brain.inits = 0; brain.produceOutput = false;
    CBrainOutput out;

    // In the zone, frame 3 % 2 != 0 and nothing active: not due.
    Check(!brain.Update(10, out), "not due");
    Check(brain.inits == 1 && brain.InitialisedCreaturePosition, "creature position initialised once");
    Check(brain.PActiveStateGroup == 0 && brain.processes == 0, "nothing ran");

    // Out of zone (period 1): picks the candidate, starts it and processes.
    Check(!brain.Update(500, out), "no output");
    Check(brain.PActiveStateGroup == &a && a.starts == 1 && a.resets == 1, "switched to candidate");
    Check(brain.processes == 1 && brain.StateGroupUpdated, "processed active group");

    // Output produced: merged and returns true.
    brain.produceOutput = true;
    Check(brain.Update(500, out), "output copied");
    Check(out.value == 42 && out.merges == 1, "merge happened");

    // Active group stops being runnable: terminated and stopped, output from the run is copied.
    a.runnable = false;
    brain.candidate = 0;
    Check(brain.Update(500, out), "terminated group still returns its output");
    Check(out.merges == 2, "merge once more");
    a.runnable = true;
    brain.produceOutput = false;

    // Terminated group with no output: stop, active cleared.
    brain.PActiveStateGroup = &a; a.Terminated = false; a.runnable = false;
    Check(!brain.Update(500, out), "no output on stop");
    Check(brain.PActiveStateGroup == 0 && brain.PLastStateGroup == &a && a.stops == 1, "stopped group cleared");

    // Creature flag 2 blocks the update.
    a.runnable = true; brain.candidate = &a;
    creature.CreatureFlags = 2;
    int before = brain.processes;
    Check(!brain.Update(500, out), "blocked");
    Check(brain.processes == before && brain.PActiveStateGroup == &a, "switch done but no process");
    creature.CreatureFlags = 0;

    // Empty state group list.
    brain.groupCount = 0;
    Check(!brain.Update(500, out), "empty list");

    if (failures == 0)
        std::printf("0088bcb0_TEST PASS\n");
    return failures;
}
