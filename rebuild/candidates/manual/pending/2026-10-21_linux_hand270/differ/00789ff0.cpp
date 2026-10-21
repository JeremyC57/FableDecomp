// Grade: DIFFER(451v451) -- sound block: retail loads the CTCSoundPlayer component into ecx before testing BowDrawbackSoundID (eax); ours sinks the component load past the id test (GetInterface form keeps it but in eax/edx)
// CTCHeroRangedAttacks::CancelAimingWeapon() -- abandon the aim: reset timers, pressures and cycle flags, recompute damage and
// accuracy for zero build-up, drop the projectile carried in the weapon's carry slot, unload a non-analogue projectile weapon
// (resetting its animation), stop the bow draw-back sound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789FF0, 449 bytes (+ int3 padding), `ret`.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                         // +0x91 bit 0: dead
    void Kill(bool immediately);                     // 0x004C9B80
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
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

struct CTCCarrying {                                 // interface type 0x46
    CThing* GetThingInPrimarySlot();                 // 0x006A1440
    CThing* RemoveThingInCarrySlot(long slot, bool drop);   // 0x006A16A0
};
struct CTCWeapon {                                   // interface type 0x48
    long GetAmmoCarrySlot();                         // 0x005DA920 (catalog "CCategory::GetCategoryIdentifier")
    bool IsProjectileWeapon();                       // 0x005DAA40
    bool IsAnalogueLoading();                        // 0x005DA950
    void SetProjectileWeaponLoaded(bool loaded, CThing* ammo);   // 0x005DAC50
};
struct CTCAnimatingObject {                          // interface type 0x83
    void PlayDefaultAnimation();                     // 0x007F8F30
};
struct CTCSoundPlayer {                              // interface type 0x10
    void StopSample(unsigned long sampleID);         // 0x0066ADD0
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();
    mutable long RefCount;                                   // +0x04
};
struct CHeroCombatDef : CDefPointeeBase { };
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer();
};
float __fastcall GetDamageMultiplierFromThingAtBuildUpTime(CThing* thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime);    // 0x00789CE0
float __fastcall GetAccuracyPercentageFromThingAtBuildUpTime(CThing* thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime);  // 0x00789D90

struct CThingCreatureBase;
struct CTCHeroRangedAttacks {
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
    if (ParentThing->HasTC(0x10)) {
        CTCSoundPlayer* sound = ParentThing->FindTC<CTCSoundPlayer>(0x10);
        if (BowDrawbackSoundID)
            sound->StopSample(BowDrawbackSoundID);
    }
    BowDrawbackSoundID = 0;
}
