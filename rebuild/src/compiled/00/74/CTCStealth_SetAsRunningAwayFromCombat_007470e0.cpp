// CTCStealth::SetAsRunningAwayFromCombat() -- start the running-away timer: the def's hang time in frames, or one frame without a def
// Leaf: SetAsRunningAwayFromCombat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007470E0, length 45 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (float to long). Data: 0x01375550 is the long frames-per-second constant (15),
// 0x0122DED8 is 1.0f.
// Layout: Ego_r.pdb CTCStealth with the retail 12-byte vector (as the landed ~CTCStealth 0x007495D0): Sneaking +0x0f,
// PDef +0x1c, RunningAwayFromCombatTimer +0x30, InDangerCoolOffTimer +0x34, DangerThings first/last +0x38/+0x3c
// (16-byte pair<CIntelligentPointer<CThing const>, CDangerStatus> elements).
// Name and signature confirmed by ghidra_out/egor_signatures.tsv (Ego_r).
// Shape: `fild [fps] ; fmul [def+44h]` = `def->Hangtime * fps` (right operand loaded first); the null-def fallback 1.0f is
// a float arm of the same ternary, so each arm converts with its own __ftol2 call (VC tail-duplicates the store).
extern long g_FramesPerSecond;                              // 0x01375550
class CThing;

struct CDangerThing {                                       // pair<CIntelligentPointer<CThing const>, NStealth::CDangerStatus>, 16 bytes
    struct CPointer {
        void*   m_vtable;
        void*   m_pItem;
        CThing* GetPItem() const;                           // CBaseIntelligentPointer::GetPItem @ 0x00A01B50
    } first;                                                // +0x00
    long Status;                                            // +0x08
    long Timer;                                             // +0x0c
};

class CStealthDef { public:
    unsigned char _pad[0x44];
    float         RunningAwayFromCombatHangtimeSeconds;     // +0x44
};

class CTCStealth { public:
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
    void SetAsRunningAwayFromCombat();
};

void CTCStealth::SetAsRunningAwayFromCombat()
{
    RunningAwayFromCombatTimer = (long)(PDef ? PDef->RunningAwayFromCombatHangtimeSeconds * g_FramesPerSecond : 1.0f);
}