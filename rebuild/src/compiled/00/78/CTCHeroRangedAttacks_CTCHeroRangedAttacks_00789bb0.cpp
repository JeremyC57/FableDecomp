// CTCHeroRangedAttacks::CTCHeroRangedAttacks(CThing* thing) -- CTCBase(thing), own vtable, null combat def, no fire cycle, timers -1, accuracy 100, damage multiplier 1, button pressures 0.01, Z-lock pressure -1, zoom mode -1
// Leaf: ??0CTCHeroRangedAttacks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789BB0, length 101 bytes (`ret 4`). Catalog label "CTCOpinionSource::CTCOpinionSource"
// is propagated: the vtable store 0x0126BAF4 is RTTI .?AVCTCHeroRangedAttacks@@ (CTCBase, CBaseClassNonCopyable, CBaseClass); the
// only caller is the component factory @ 0x004D4A25.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated).
// Layout = PDB CTCHeroRangedAttacks (0x4C): PParentCreature +0x0C (not initialised here), PHeroCombatDef +0x10, FireCycleInProgress/
// Aiming/FireQueued/Loading +0x14..+0x17, AimingTimer +0x18, PullBackTimer +0x1C, PullBackAmount +0x20, Accuracy +0x24,
// DamageMultiplier +0x28, Decapitate +0x2C, ButtonReleaseCancelTimer +0x30, ButtonPressure +0x34, LastButtonPressure +0x38,
// ProjectileWeaponZLockPressure +0x3C, ProjectileWeaponZoomParameter +0x40, ProjectileWeaponFirstPersonZoomModeIndex +0x44,
// BowDrawbackSoundID +0x48.
// Shape: init list in declaration order; VC groups the stores by register (zeros in eax, -1 in ecx, 0.01f in edx, other floats
// as immediates).
struct CThing;
struct CThingCreatureBase;
struct CHeroCombatDef;
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
struct CTCHeroRangedAttacks : CTCBase {             // vtable 0x0126BAF4
    CThingCreatureBase* PParentCreature;            // +0x0c
    CDefPointer<const CHeroCombatDef> PHeroCombatDef;   // +0x10
    bool  FireCycleInProgress;                      // +0x14
    bool  Aiming;                                   // +0x15
    bool  FireQueued;                               // +0x16
    bool  Loading;                                  // +0x17
    long  AimingTimer;                              // +0x18
    long  PullBackTimer;                            // +0x1c
    float PullBackAmount;                           // +0x20
    float Accuracy;                                 // +0x24
    float DamageMultiplier;                         // +0x28
    bool  Decapitate;                               // +0x2c
    long  ButtonReleaseCancelTimer;                 // +0x30
    float ButtonPressure;                           // +0x34
    float LastButtonPressure;                       // +0x38
    float ProjectileWeaponZLockPressure;            // +0x3c
    float ProjectileWeaponZoomParameter;            // +0x40
    long  ProjectileWeaponFirstPersonZoomModeIndex; // +0x44
    unsigned long BowDrawbackSoundID;               // +0x48
    virtual void v00();
    CTCHeroRangedAttacks(CThing* thing);
};

CTCHeroRangedAttacks::CTCHeroRangedAttacks(CThing* thing)
    : CTCBase(thing)
    , FireCycleInProgress(false)
    , Aiming(false)
    , FireQueued(false)
    , Loading(false)
    , AimingTimer(-1)
    , PullBackTimer(-1)
    , PullBackAmount(0.0f)
    , Accuracy(100.0f)
    , DamageMultiplier(1.0f)
    , Decapitate(false)
    , ButtonReleaseCancelTimer(0)
    , ButtonPressure(0.01f)
    , LastButtonPressure(0.01f)
    , ProjectileWeaponZLockPressure(-1.0f)
    , ProjectileWeaponZoomParameter(0.0f)
    , ProjectileWeaponFirstPersonZoomModeIndex(-1)
    , BowDrawbackSoundID(0)
{
}