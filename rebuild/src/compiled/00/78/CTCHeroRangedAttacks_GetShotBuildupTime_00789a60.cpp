// CTCHeroRangedAttacks::GetShotBuildupTime() const -- seconds the hero has spent aiming the current shot (aiming timer frames / frames per second)
// Leaf: ?GetShotBuildupTime@CTCHeroRangedAttacks@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789A60, 10 bytes, plain `ret` (float on the x87 stack).
// Identity: Ego_r.pdb `?GetShotBuildupTime@CTCHeroRangedAttacks@@QBEMXZ`, by the donor TU order (GetProjectileTargetingZoomParameter
//   0x790A30, GetShotBuildupTime 0x790A40, GetPullBackAmount 0x790A60, GetShotAccuracyFraction 0x790A70; retail 0x00789A50 reads
//   ProjectileWeaponZoomParameter +0x40, 0x00789A70 PullBackAmount +0x20). Its caller 0x00899C5F fetches the component as
//   interface 0x81 (CTCHeroRangedAttacks::GetInterfaceType 0x0078AF40). Catalog label `CTCTalk::GetCurrentSpeechDuration`
//   was propagated (CTCTalk +0x18 is its Attitude enum).
// Layout: as the landed CTCHeroRangedAttacks ctor 0x00789BB0 (Ego_r.pdb CTCHeroRangedAttacks, 0x4C bytes, no containers so
//   the PDB offsets hold in retail; no engine header exists for the class).
// Globals: g_framesPerSecond 0x01375550 (long, 15).
// Shape: a long divided as float by a long global: `fild [this+18h] ; fidiv [g]`.
extern long g_framesPerSecond;                  // 0x01375550
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
    float GetShotBuildupTime() const;
};

float CTCHeroRangedAttacks::GetShotBuildupTime() const
{
    return (float)AimingTimer / g_framesPerSecond;
}