// CTCStealth::ClearDangerStatus() -- reset the in-danger cool-off timer; for the main player's creature also switch the danger music off and rain back on
// Leaf: ClearDangerStatus
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747090, length 74 bytes.
// Callees: CThing::GetPlayer @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520, CTCBase::GetWorld() @ 0x00686A70 (twice;
// manifest "CViewBase::GetDisplayEngine" is propagated), CMusicManager::EnableDangerMusic(bool) @ 0x006BDE50 (manifest
// "CGameEvent::AddUBYTE", propagated; Ego_r has ?EnableDangerMusic@CMusicManager@@QAEX_N@Z), CEnvironment::SetRainEnabled
// (bool) @ 0x006B32B0 (manifest label; the only bool setter of CEnvironment in Ego_r).
// CWorld (Ego_r.pdb): PEnvironment (scoped_ptr<CEnvironment>) +0x18, PMusicManager (CCountedPointer) +0x6c; the thing's
// type byte +0x10 is 2 for a creature.
// Layout: Ego_r.pdb CTCStealth with the retail 12-byte vector (as the landed ~CTCStealth 0x007495D0): Sneaking +0x0f,
// PDef +0x1c, RunningAwayFromCombatTimer +0x30, InDangerCoolOffTimer +0x34, DangerThings first/last +0x38/+0x3c
// (16-byte pair<CIntelligentPointer<CThing const>, CDangerStatus> elements).
// Name and signature confirmed by ghidra_out/egor_signatures.tsv (Ego_r).
// Shape: GetWorld() is re-called for each chained member call (an out-of-line getter); each result goes through a named
// local so the call precedes the argument push (the bare chain pushes the argument first).
struct CPlayer {
    bool IsMainPlayer() const;                              // 0x00487520
};

struct CMusicManager {
    void EnableDangerMusic(bool enable);                    // 0x006BDE50
};

struct CEnvironment {
    void SetRainEnabled(bool enabled);                      // 0x006B32B0
};

struct CWorld {
    unsigned char  _pad00[0x18];
    CEnvironment*  PEnvironment;                            // +0x18 scoped_ptr<CEnvironment>
    unsigned char  _pad1c[0x6c - 0x1c];
    CMusicManager* PMusicManager;                           // +0x6c CCountedPointer<CMusicManager> (object word)
};

struct CThing {
    unsigned char _pad[0x10];
    unsigned char m_thingType;                              // +0x10 (2 = creature)
    CPlayer* GetPlayer() const;                             // 0x004C7A30
};

struct CDangerThing {                                       // pair<CIntelligentPointer<CThing const>, NStealth::CDangerStatus>, 16 bytes
    struct CPointer {
        void*   m_vtable;
        void*   m_pItem;
        CThing* GetPItem() const;                           // CBaseIntelligentPointer::GetPItem @ 0x00A01B50
    } first;                                                // +0x00
    long Status;                                            // +0x08
    long Timer;                                             // +0x0c
};

struct CStealthDef {
    unsigned char _pad[0x44];
    float         RunningAwayFromCombatHangtimeSeconds;     // +0x44
};

struct CTCStealth {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    bool          Viewable;                                 // +0x0c
    bool          Smellable;                                // +0x0d
    bool          Hearable;                                 // +0x0e
    bool          Sneaking;                                 // +0x0f
    bool          SneakingToggleSet;                        // +0x10
    float         DistanceCanBeHeardFrom;                   // +0x14
    float         MaxSneakingSpeed;                         // +0x18
    CStealthDef*  PDef;                                     // +0x1c CDefPointer<CStealthDef const>
    long          FrameStartedWalking;                      // +0x20
    long          FrameStartedSneakingAndMoving;            // +0x24
    long          SneakingAndMovingDeedHandle;              // +0x28
    long          ActionPermittedToSneakID;                 // +0x2c
    long          RunningAwayFromCombatTimer;               // +0x30
    long          InDangerCoolOffTimer;                     // +0x34
    CDangerThing* m_pDangerFirst;                           // +0x38 DangerThings (retail 12-byte vector)
    CDangerThing* m_pDangerLast;                            // +0x3c
    CDangerThing* m_pDangerEnd;                             // +0x40
    long          MaxNumberOfDangerThings;                  // +0x44
    long          RegionBasedMaxNumberOfDangerThings;       // +0x48
    CWorld* GetWorld();                                     // 0x00686A70 (CTCBase)
    void ClearDangerStatus();
};

void CTCStealth::ClearDangerStatus()
{
    InDangerCoolOffTimer = -1;
    if (ParentThing->GetPlayer()->IsMainPlayer() && ParentThing->m_thingType == 2) {
        CWorld* world = GetWorld();
        world->PMusicManager->EnableDangerMusic(false);
        CWorld* world2 = GetWorld();
        world2->PEnvironment->SetRainEnabled(true);
    }
}
