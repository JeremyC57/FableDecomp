// CTCHeroRangedAttacks::CancelAimingWeapon() -- abandon the aim: reset timers, pressures and cycle flags, recompute damage and
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// accuracy for zero build-up, drop the projectile carried in the weapon's carry slot, unload a non-analogue projectile weapon
// (resetting its animation), stop the bow draw-back sound
// Shape: GetInterface stores the component through the out-pointer as ONE ternary
//   `*ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;` (the landed SetupDressingAtLevel spelling),
//   and the sound player is fetched with GetInterface (not HasTC + FindTC): VC7.1 then loads the component into ecx before the
//   BowDrawbackSoundID test as retail; the CVectorMap LowerBound instantiation is defined in this TU (noinline).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789FF0, 449 bytes (+ int3 padding), `ret`.
#include "fable_compiler.h"
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                         // +0x91 bit 0: dead
    void Kill(bool immediately);                     // 0x004C9B80
    FABLE_FORCEINLINE unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
    }
};

inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCCarrying { public:                                 // interface type 0x46
    CThing* RemoveThingInCarrySlot(long slot, bool drop);   // 0x006A16A0
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CTCWeapon { public:                                   // interface type 0x48
    long GetAmmoCarrySlot();                         // 0x005DA920 (catalog "CCategory::GetCategoryIdentifier")
    bool IsAnalogueLoading();                        // 0x005DA950
    void SetProjectileWeaponLoaded(bool loaded, CThing* ammo);   // 0x005DAC50
public:
    bool IsProjectileWeapon() const;   // 0x005DAA40
};
class CTCAnimatingObject { public:                          // interface type 0x83
    void PlayDefaultAnimation();                     // 0x007F8F30
};
class CTCSoundPlayer { public:                              // interface type 0x10
public:
    void StopAttachedSound(unsigned long);   // 0x0066ADD0
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();
    mutable long RefCount;                                   // +0x04
};
class CHeroCombatDef : public CDefPointeeBase { public: };
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer();
};
float __fastcall GetDamageMultiplierFromThingAtBuildUpTime(CThing* thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime);    // 0x00789CE0
float __fastcall GetAccuracyPercentageFromThingAtBuildUpTime(CThing* thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime);  // 0x00789D90

class CThingCreatureBase;
class CTCHeroRangedAttacks { public:
    void*               __vftable;                   // +0x00
    CThing*             ParentThing;                 // +0x04 CTCBase
    unsigned char       _pad_0x08[0x04];
    CThingCreatureBase* PParentCreature;             // +0x0C
    CDefPointer<const CHeroCombatDef> PHeroCombatDef;   // +0x10
    bool                FireCycleInProgress;         // +0x14
    bool                Aiming;                      // +0x15
    bool                FireQueued;                  // +0x16
    bool                Loading;                     // +0x17
    long                AimingTimer;                 // +0x18
    long                PullBackTimer;               // +0x1c
    float               PullBackAmount;              // +0x20
    float               Accuracy;                    // +0x24
    float               DamageMultiplier;            // +0x28
    bool                Decapitate;                  // +0x2c
    long                ButtonReleaseCancelTimer;    // +0x30
    float               ButtonPressure;              // +0x34
    float               LastButtonPressure;          // +0x38
    float               ProjectileWeaponZLockPressure;   // +0x3c
    float               ProjectileWeaponZoomParameter;   // +0x40
    long                ProjectileWeaponFirstPersonZoomModeIndex;   // +0x44
    unsigned long       BowDrawbackSoundID;          // +0x48
    void CancelAimingWeapon();
};

void CTCHeroRangedAttacks::CancelAimingWeapon()
{
    AimingTimer = -1;
    PullBackTimer = -1;
    ButtonPressure = 0.01f;
    LastButtonPressure = 0.01f;
    FireCycleInProgress = false;
    Aiming = false;
    FireQueued = false;
    Loading = false;
    ButtonReleaseCancelTimer = 0;
    PullBackAmount = 0.0f;
    DamageMultiplier = GetDamageMultiplierFromThingAtBuildUpTime(ParentThing, PHeroCombatDef, 0.0f);
    Accuracy = GetAccuracyPercentageFromThingAtBuildUpTime(ParentThing, PHeroCombatDef, 0.0f);
    CTCCarrying* carrying;
    if (ParentThing->GetInterface(&carrying, 0x46)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(weapon) && weapon->HasTC(0x48)) {
            CTCWeapon* tc = weapon->FindTC<CTCWeapon>(0x48);
            long slot = tc->GetAmmoCarrySlot();
            if (slot >= 0) {
                CThing* ammo = carrying->RemoveThingInCarrySlot(slot, false);
                if (IS_THING_ALIVE(ammo))
                    ammo->Kill(true);
            }
            if (tc->IsProjectileWeapon() && !tc->IsAnalogueLoading()) {
                if (weapon->HasTC(0x83))
                    weapon->FindTC<CTCAnimatingObject>(0x83)->PlayDefaultAnimation();
                tc->SetProjectileWeaponLoaded(false, 0);
            }
        }
    }
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10)) {
        if (BowDrawbackSoundID)
            sound->StopAttachedSound(BowDrawbackSoundID);
    }
    BowDrawbackSoundID = 0;
}