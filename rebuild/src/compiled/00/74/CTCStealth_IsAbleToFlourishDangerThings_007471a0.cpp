// CTCStealth::IsAbleToFlourishDangerThings() const -- true when flourishing is off, or some danger thing is alive, not knocked down and not in status 4
// Leaf: IsAbleToFlourishDangerThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007471A0, length 80 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, IsKnockedDown(CThing*) @ 0x0085F010 (`__fastcall`, manifest).
// Data: 0x0137574D is a global bool (the flourish switch).
// Layout: Ego_r.pdb CTCStealth with the retail 12-byte vector (as the landed ~CTCStealth 0x007495D0): Sneaking +0x0f,
// PDef +0x1c, RunningAwayFromCombatTimer +0x30, InDangerCoolOffTimer +0x34, DangerThings first/last +0x38/+0x3c
// (16-byte pair<CIntelligentPointer<CThing const>, CDangerStatus> elements).
// Name and signature confirmed by ghidra_out/egor_signatures.tsv (Ego_r).
// Shape: `last` re-read every iteration; the thing pointer is a named local.
extern bool g_FlourishDangerThings;                         // 0x0137574D
struct CThing;
bool __fastcall IsKnockedDown(CThing* thing);               // 0x0085F010

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
    bool IsAbleToFlourishDangerThings() const;
};

bool CTCStealth::IsAbleToFlourishDangerThings() const
{
    if (!g_FlourishDangerThings)
        return true;
    for (CDangerThing* it = m_pDangerFirst; it != m_pDangerLast; ++it) {
        CThing* thing = it->first.GetPItem();
        if (thing && !IsKnockedDown(thing) && it->Status != 4)
            return true;
    }
    return false;
}