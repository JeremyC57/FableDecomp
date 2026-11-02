// CTCStealth::OnCreate() -- on creation reset the stealth state (not viewable / smellable / hearable / sneaking, the walking, sneaking, running-away and danger timers unset) and derive the sneaking speed
// Leaf: ?OnCreate@CTCStealth@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747530, 36 bytes, thiscall, tail jump to InitialiseSneakSpeed.
// Identity (proven): RTTI vtable 0x012689EC (.?AVCTCStealth@@) slot 4, its only reference; slot 4 is OnCreate in every CTCBase
//   vtable (13 landed rows); Ego_r.pdb holds ?OnCreate@CTCStealth@@UAEXXZ. The catalog label "vfunc_4" is a placeholder.
// Callees: CTCStealth::ResetStealthVariables() (Ego_r ?ResetStealthVariables@CTCStealth@@AAEXXZ, private), defined above in the
//   same file and inlined by VC7.1; retail's out-of-line copy is the unreferenced byte twin 0x00747500 just before this row (the
//   inline-everywhere-but-once pattern). CTCStealth::InitialiseSneakSpeed() 0x00747330 (Ego_r
//   ?InitialiseSneakSpeed@CTCStealth@@AAEXXZ, private; the landed row spells it public QAE).
// Layout: Ego_r.pdb CTCStealth (retail = PDB up to +0x38, as the landed StartSneaking 0x00747560): Viewable +0x0C, Smellable +0x0D,
//   Hearable +0x0E, Sneaking +0x0F, FrameStartedWalking +0x20, FrameStartedSneakingAndMoving +0x24, RunningAwayFromCombatTimer
//   +0x30, InDangerCoolOffTimer +0x34. engine/CTCBase.h models the vptr as a data member, so CTCBase is restated polymorphic here.
// Shape: the four byte stores share one `xor al,al`, the four -1 stores one `mov eax,-1`.
class CThing;

class CTCBase {
public:
    virtual ~CTCBase();                                     // slot 0 (the ??_G)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                                // slot 4
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCStealth : public CTCBase {
public:
    virtual void OnCreate();
private:
    void InitialiseSneakSpeed();                            // 0x00747330
    void ResetStealthVariables();                           // 0x00747500
public:
    bool  Viewable;                                         // +0x0C
    bool  Smellable;                                        // +0x0D
    bool  Hearable;                                         // +0x0E
    bool  Sneaking;                                         // +0x0F
    bool  SneakingToggleSet;                                // +0x10
    float DistanceCanBeHeardFrom;                           // +0x14
    float MaxSneakingSpeed;                                 // +0x18
    void* PDef;                                             // +0x1C CDefPointer<CStealthDef const>
    long  FrameStartedWalking;                              // +0x20
    long  FrameStartedSneakingAndMoving;                    // +0x24
    long  SneakingAndMovingDeedHandle;                      // +0x28
    long  ActionPermittedToSneakID;                         // +0x2C
    long  RunningAwayFromCombatTimer;                       // +0x30
    long  InDangerCoolOffTimer;                             // +0x34
};

void CTCStealth::ResetStealthVariables()
{
    Viewable = false;
    Smellable = false;
    Hearable = false;
    Sneaking = false;
    FrameStartedWalking = -1;
    FrameStartedSneakingAndMoving = -1;
    RunningAwayFromCombatTimer = -1;
    InDangerCoolOffTimer = -1;
    InitialiseSneakSpeed();
}

void CTCStealth::OnCreate()
{
    ResetStealthVariables();
}