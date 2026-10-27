// CTCHeroRangedAttacks::AttemptToLoadRangedWeapon() -- start a ranged fire cycle: mark the cycle in progress and the hero as aiming
// Leaf: ?AttemptToLoadRangedWeapon@CTCHeroRangedAttacks@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007899D0, 9 bytes, plain `ret`.
// Identity: Ego_r.pdb `?AttemptToLoadRangedWeapon@CTCHeroRangedAttacks@@QAEXXZ`. The Ego_r.pdb donor lays the
//   CTCHeroRangedAttacks TU out in the same order as retail (AttemptToLoadRangedWeapon 0x7909B0, SetButtonPressure
//   0x7909C0, SetProjectileTargetingZoomDelta, GetProjectileTargetingZoomParameter, GetShotBuildupTime, GetPullBackAmount,
//   GetShotAccuracyFraction ...); retail 0x007899D0 sits right before the landed SetButtonPressure 0x007899E0. The only
//   caller (0x0068F0A2, the hero input handler) calls it when the ranged-attack button event fires and then hands the
//   button pressure to SetButtonPressure. Catalog label `CMeleeApprentice::Init` was propagated.
// Layout: as the landed CTCHeroRangedAttacks ctor 0x00789BB0 (Ego_r.pdb CTCHeroRangedAttacks, 0x4C bytes, no containers so
//   the PDB offsets hold in retail; no engine header exists for the class).
// Shape: two byte stores from one `mov al,1`.
class CThing;
class CThingCreatureBase;
class CHeroCombatDef;
template<class T> class CDefPointer { public:
    T* Object;
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
};
class CTCHeroRangedAttacks : public CTCBase { public:             // vtable 0x0126BAF4
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
    void AttemptToLoadRangedWeapon();
};

void CTCHeroRangedAttacks::AttemptToLoadRangedWeapon()
{
    FireCycleInProgress = true;
    Aiming = true;
}