// CTCWeapon::CTCWeapon(CThing*) -- CTCBase(thing), own vtable, no def, unloaded, not aiming, nothing killed, not wielded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA6A0, length 52 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated).
// Vtable 0x012554FC = CTCWeapon (rtti-vtables.tsv). Called by the factory 0x004D3CA3.
// Layout: engine/CTCWeapon.h (Ego_r.pdb, 0x28 bytes): WeaponDef +0x0c (CDefPointer), ProjectileWeaponLoaded +0x10,
// AimingTime +0x14, AimingTimeOverride +0x18, NumThingsKilled +0x1c, TimeWielded +0x20, Wielded +0x24. The header
// flattens the CTCBase base into pads, so a constructor needs the local model below (same offsets).
// Shape: one zero register feeds the def pointer's inline constructor and the zero/false initialisers in
// declaration order; AimingTime = -1 is assigned in the body (its immediate store comes last).
struct CThing;
struct CWeaponDef;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CDefPointer_CWeaponDef {
    const CWeaponDef* Object;
    CDefPointer_CWeaponDef() : Object(0) { }
};

struct CTCWeapon : CTCBase_Calls {                          // vtable 0x012554FC
    CDefPointer_CWeaponDef WeaponDef;                       // +0x0c
    bool                   ProjectileWeaponLoaded;          // +0x10
    long                   AimingTime;                      // +0x14
    bool                   AimingTimeOverride;              // +0x18
    long                   NumThingsKilled;                 // +0x1c
    long                   TimeWielded;                     // +0x20
    bool                   Wielded;                         // +0x24
    virtual void v00();
    CTCWeapon(CThing* thing);
};

CTCWeapon::CTCWeapon(CThing* thing)
    : CTCBase_Calls(thing)
    , ProjectileWeaponLoaded(false)
    , AimingTimeOverride(false)
    , NumThingsKilled(0)
    , TimeWielded(0)
    , Wielded(false)
{
    AimingTime = -1;
}