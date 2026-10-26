// CTCTrophy::CTCTrophy(CThing*) -- trophy thing component: CTCBase(parent), null def pointer, no deed handle, zeroed minigame counters, two empty witness lists, cleared wait flags, Mountable = true; then force a frame update on a parent that is not yet flagged
// Leaf: ??0CTCTrophy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DDAE0, length 148 bytes (+ int3 padding). Catalogued as
// "CEngineSubPrimitive2DClockSprite::Constructor" (a propagated label); the vtable it stores, 0x0125FC54, is CTCTrophy's
// (rtti-vtables.tsv), and the member layout is the PDB CTCTrophy with retail 4-byte lists.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (twice, one 0x10-byte sentinel node per list),
// CThing::SetForceFrameUpdate(bool) @ 0x004C7C60.
// Layout (PDB names): PDef +0x0c (CDefPointer, null), NonMinigameDeedHandle +0x10 (-1), MinigameInProgress +0x14,
// six counters +0x18..+0x2c, FrameForNextGainedWitnessSound +0x30, AvailableWitnesses +0x34 and WitnessesAchieved
// +0x38 (retail single-pointer sentinel lists; PDB 12 bytes each, so everything after sits 16 bytes lower than the PDB),
// four Waiting* flags +0x3c..+0x3f, IsGlobalTC +0x40, CurrentInstructionsPage +0x44, Mountable +0x48.
// Shape: initialiser list in declaration order, the zero in ebx; each inline list constructor nulls its head before
// malloc and links the node to itself before storing it; the two malloc pushes are popped together (`add esp,8`)
// after the parent's +0x70 load, which starts the body.
extern "C" void* __cdecl malloc(unsigned int size);

struct CThingPhysics_0x70 {
    unsigned char _pad[0x41];
    unsigned char m_flags;                         // +0x41 (bit 1: already frame-updating)
};

class CThing { public:
    unsigned char       _pad[0x70];
    CThingPhysics_0x70* m_pState;                  // +0x70
    void SetForceFrameUpdate(bool force);          // 0x004C7C60
};

class CTCBase { public:
    virtual void v00();
    unsigned char _body[0x8];                      // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase(CThing* parent);                       // 0x00686800
};

struct CDefPointer_CTrophyDef {
    const void* m_pDef;
    CDefPointer_CTrophyDef() : m_pDef(0) {}
};

struct CRetailList_node0x10 {
    struct node { node* next; node* prev; unsigned char value[8]; };   // CIntelligentPointer<CThingAICreature>
    node* head;
    CRetailList_node0x10() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

class CTCTrophy : public CTCBase { public:
    CDefPointer_CTrophyDef PDef;                   // +0x0c
    long                   NonMinigameDeedHandle;  // +0x10
    bool                   MinigameInProgress;     // +0x14
    long                   BestWitnessesAheadToDate;        // +0x18
    long                   MinigameInitialFramesLeft;       // +0x1c
    long                   MinigameCurrentFramesLeft;       // +0x20
    long                   MinigameStartingWitnessCount;    // +0x24
    long                   NumberOfWitnessesLeft;           // +0x28
    long                   PendingGainedWitnessSounds;      // +0x2c
    long                   FrameForNextGainedWitnessSound;  // +0x30
    CRetailList_node0x10   AvailableWitnesses;     // +0x34
    CRetailList_node0x10   WitnessesAchieved;      // +0x38
    bool                   WaitingForConfirmation; // +0x3c
    bool                   WaitingForScore;        // +0x3d
    bool                   WaitingForTerminate;    // +0x3e
    bool                   WaitingForNextInstructionsPage;  // +0x3f
    bool                   IsGlobalTC;             // +0x40
    long                   CurrentInstructionsPage;         // +0x44
    bool                   Mountable;              // +0x48
    virtual void v00();                            // own vtable 0x0125FC54
    CTCTrophy(CThing* parent);
};

CTCTrophy::CTCTrophy(CThing* parent)
    : CTCBase(parent)
    , NonMinigameDeedHandle(-1)
    , MinigameInProgress(false)
    , BestWitnessesAheadToDate(0)
    , MinigameInitialFramesLeft(0)
    , MinigameCurrentFramesLeft(0)
    , MinigameStartingWitnessCount(0)
    , NumberOfWitnessesLeft(0)
    , PendingGainedWitnessSounds(0)
    , FrameForNextGainedWitnessSound(0)
    , WaitingForConfirmation(false)
    , WaitingForScore(false)
    , WaitingForTerminate(false)
    , WaitingForNextInstructionsPage(false)
    , IsGlobalTC(false)
    , CurrentInstructionsPage(0)
    , Mountable(true)
{
    if (parent->m_pState && !(parent->m_pState->m_flags & 2))
        parent->SetForceFrameUpdate(true);
}