// CAIBrain::Update(long, CBrainOutput&) -- pick the update zone for the distance, run the active state group on its zone's update period, switch to a pending higher-priority group, update the active group and copy any valid output
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088BCB0, 640 bytes (ret 8). Name and signature from Ego_r.pdb (?Update@CAIBrain@@QAE_NJAAVCBrainOutput@@@Z).
// Callees: GetUpdateZone(long) const 0x0088B910, the creature's ready test 0x00833710, InitCreaturePosition 0x0088BBB0,
//   CMainGameComponent world-frame getter 0x004167C8, _aullrem 0x00BFEBB0, FindNextStateGroupToUse(long) 0x0088B870,
//   CAIStateGroupBase::Terminate 0x008FCF40 / reset 0x008FCF50 / stop 0x008FD050, CBrainOutput merge 0x0088C840,
//   CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0, the brain's virtual slot 1 (SuspendableProcess).
// Layout (retail, 12-byte STLport vectors, PDB offsets less 4 from StateGroupList on): StateGroupList +0x28,
//   PActiveStateGroup +0x38, PLastStateGroup +0x3c, PPendingStateGroup +0x40, Output +0x44, OutputValid +0x8c,
//   UpdatingStateGroup +0x8d, StateGroupUpdated +0x8e, InitialisedCreaturePosition +0x8f, PCurrentUpdateZone +0xa0.
// Shape: the period test is a __forceinline helper that names the creature's 64-bit offset before the frame call (retail
//   loads it into callee-saved registers first); the group-name copy sits in its own {} block so it shares the dead
//   `distance` argument slot with the top-level output flag (retail frame is one word).
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& o);                  // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CBrainOutput {
    unsigned char _body[0x48];
    void Merge(const CBrainOutput& other);              // 0x0088C840
};
struct CMainGameComponent {
    unsigned long GetWorldFrame() const;                // 0x004167C8
};
struct CThingAICreature {
    unsigned char _pad00[0x18];
    unsigned __int64 UpdateOffset;                      // +0x18
    unsigned char _pad20[0x1c8 - 0x20];
    unsigned char CreatureFlags;                        // +0x1c8
    bool IsReadyForBrain() const;                       // 0x00833710
};
struct CBrainUpdateZone {
    long Unknown;
    unsigned long UpdatePeriod;                         // +4
    unsigned long PriorityCheckPeriod;                  // +8
};
struct CAIStateGroupBase {
    virtual void v00();
    virtual void v01();
    virtual void Start();                               // slot 2
    virtual void v03();
    virtual void v04();
    virtual bool IsRunnable();                          // slot 5
    virtual void v06();
    virtual bool UpdateEveryFrame();                    // slot 7
    void* PBrain;
    long ScriptAIPriority;
    CCharString Name;                                   // +0x0c
    bool Terminated;                                    // +0x10
    unsigned char Priority;                             // +0x11
    void Terminate();                                   // 0x008FCF40
    void Reset();                                       // 0x008FCF50
    void Stop();                                        // 0x008FD050
};
struct CAIBrain {
    virtual void v00();
    virtual void SuspendableProcess();                  // slot 1
    unsigned char _pad04[0x14 - 4];
    CMainGameComponent* Component;                      // +0x14
    CThingAICreature* Creature;                         // +0x18
    unsigned char _pad1c[0x28 - 0x1c];
    CAIStateGroupBase** StateGroupsFirst;               // +0x28
    CAIStateGroupBase** StateGroupsLast;                // +0x2c
    unsigned char _pad30[0x38 - 0x30];
    CAIStateGroupBase* PActiveStateGroup;               // +0x38
    CAIStateGroupBase* PLastStateGroup;                 // +0x3c
    CAIStateGroupBase* PPendingStateGroup;              // +0x40
    CBrainOutput Output;                                // +0x44
    bool OutputValid;                                   // +0x8c
    bool UpdatingStateGroup;                            // +0x8d
    bool StateGroupUpdated;                             // +0x8e
    bool InitialisedCreaturePosition;                   // +0x8f
    unsigned char _pad90[0xa0 - 0x90];
    const CBrainUpdateZone* PCurrentUpdateZone;         // +0xa0
    const CBrainUpdateZone* GetUpdateZone(long distance) const;   // 0x0088B910
    void InitCreaturePosition();                        // 0x0088BBB0
    CAIStateGroupBase* FindNextStateGroupToUse(long priority);    // 0x0088B870
    bool Update(long distance, CBrainOutput& output);
    __forceinline bool IsFrameDue(unsigned long period) const
    {
        unsigned __int64 offset = Creature->UpdateOffset;
        return (offset + Component->GetWorldFrame()) % period == 0;
    }
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
    if (StateGroupsFirst == StateGroupsLast)
        return false;
    bool stillRunnable = true;
    if (PActiveStateGroup && !PActiveStateGroup->IsRunnable()) {
        PActiveStateGroup->Terminate();
        stillRunnable = false;
    }
    if (PActiveStateGroup && !PActiveStateGroup->Terminated
        && IsFrameDue(priorityPeriod)) {
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
