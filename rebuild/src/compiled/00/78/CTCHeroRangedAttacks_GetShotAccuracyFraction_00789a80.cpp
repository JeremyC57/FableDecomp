// CTCHeroRangedAttacks::GetShotAccuracyFraction() const -- the shot accuracy (a percentage) as a 0..1 fraction
// Leaf: ?GetShotAccuracyFraction@CTCHeroRangedAttacks@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789A80, 10 bytes, plain `ret` (float on the x87 stack).
// Identity: Ego_r.pdb `?GetShotAccuracyFraction@CTCHeroRangedAttacks@@QBEMXZ`, by the donor TU order (GetPullBackAmount
//   0x790A60 then GetShotAccuracyFraction 0x790A70; retail 0x00789A70 returns PullBackAmount +0x20, 0x00789A80 scales
//   Accuracy +0x24, which the landed ctor initialises to 100). Caller 0x0080358C fetches interface 0x81 (CTCHeroRangedAttacks)
//   and calls GetPullBackAmount then this. Catalog label `CMap::DrawGetSeaLevel` was propagated.
// Layout: as the landed CTCHeroRangedAttacks ctor 0x00789BB0 (Ego_r.pdb CTCHeroRangedAttacks, 0x4C bytes, no containers so
//   the PDB offsets hold in retail; no engine header exists for the class).
// Constants: 0.01f in the pool at 0x01230A5C.
// Shape: `Accuracy * 0.01f` -- the member is loaded first and the literal multiplies from the pool.
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
    float GetShotAccuracyFraction() const;
};

float CTCHeroRangedAttacks::GetShotAccuracyFraction() const
{
    return Accuracy * 0.01f;
}