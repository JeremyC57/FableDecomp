// CTCHeroRangedAttacks::FrameUpdate() -- per-frame ranged attack: fire a queued shot, or while aiming play the bow draw sound, advance the pull-back, clamp it, cap the pull-back timer at the first spline root, set decapitation/damage/accuracy, reset the timers when the carried projectile weapon is loaded, then load/fire; otherwise finish an analogue load or cancel the aim
// Grade: DIFFER(1094v1097) -- frame 0x1c v 0x14: retail shares one key word for every lookup and keeps the root-vector start in ebp across the tail (free(ebp)); byte-aligned bitset narrowing (mov cl,[+0x22] v shr 0x10) is a lane gap
// Leaf: ?FrameUpdate@CTCHeroRangedAttacks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078A690, 1097 bytes, `ret`.
// Layout: engine/CTCHeroRangedAttacks.h offsets (vptr, ParentThing +0x04 modelled here because the header flattens CTCBase).
extern "C" void free(void*);
extern long g_FramesPerSecond;                      // 0x01375550

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CCharString {
    void* Data;
    CCharString(const char* s, long len = -1);       // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};

struct CPlayer {
    long GetCurrentMode() const;                     // 0x00633BE0
    void AddMode(long mode, bool exclusive);         // 0x006345C0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                         // +0x91 bit 0: dead
    CPlayer* GetPlayer();                            // 0x004C7A30
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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceTest(T** ppOut, long interfaceType)
    {
        bool present = (m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        GetInterface(&tc, interfaceType);
        return tc;
    }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* FindTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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

struct CTCSoundPlayer {                              // interface type 0x10
    unsigned long PlayAttachedSoundEvent(const CCharString& name, void* init, long a, long b);   // 0x0066B1D0
};
struct CTCCarrying {                                 // interface type 0x46
    CThing* GetThingInPrimarySlot();                 // 0x006A1440
};
struct CTCWeapon {                                   // interface type 0x48
    bool IsAnalogueLoading();                        // 0x005DA950
    bool IsProjectileWeaponLoaded();                 // 0x005DA980
    bool IsProjectileWeapon();                       // 0x005DAA40
};

struct CDefPointeeBase { void* __vftable; long RefCount; };
struct CHeroCombatDef;
template<class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer();
};

struct CFloatVector {                                // std::vector<float>
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    ~CFloatVector() { if (_M_start) free(_M_start); }
};
CFloatVector __fastcall BuildSpline_CubicRootFinding(const float& value, float a, float b, bool c, bool d);   // 0x0078B7C0
float __fastcall GetDamageMultiplierFromThingAtBuildUpTime(CThing* thing, CDefPointer<CHeroCombatDef> def, float time);   // 0x00789CE0
extern "C" double tanh(double);
#pragma intrinsic(tanh)

struct CThingCreatureBase;
struct CTCHeroRangedAttacks {
    void*               __vftable;                   // +0x00
    CThing*             ParentThing;                 // +0x04
    unsigned char       _pad_0x08[0x04];
    CThingCreatureBase* PParentCreature;             // +0x0C
    CDefPointer<CHeroCombatDef> PHeroCombatDef;      // +0x10
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
    bool IsAbleToUseRangedWeapon();                  // 0x00789E60
    void CancelAimingWeapon();                       // 0x00789FF0
    void LoadRangedWeapon();                         // 0x0078A2C0
    void FireRangedWeapon();                         // 0x0078A390
    void AttemptToFireRangedWeapon();                // 0x0078A5D0
    void FrameUpdate();
};

void CTCHeroRangedAttacks::FrameUpdate()
{
    DamageMultiplier = 0;
    Accuracy = 0;
    Decapitate = false;
    if (IsAbleToUseRangedWeapon() && FireCycleInProgress && !Aiming && !FireQueued) {
        AttemptToFireRangedWeapon();
        return;
    }
    if (IsAbleToUseRangedWeapon() && FireCycleInProgress && (Aiming || FireQueued)) {
        if (AimingTimer >= 0) {
            CTCSoundPlayer* sound;
            if (ParentThing->GetInterfaceTest(&sound, 0x10) && BowDrawbackSoundID == 0) {
                CCharString name("SE_DRAW_BOW");
                BowDrawbackSoundID = sound->PlayAttachedSoundEvent(name, 0, 0, 0);
            }
        }
        if (ButtonPressure >= LastButtonPressure) {
            if (AimingTimer >= 0) {
                ++AimingTimer;
                ++PullBackTimer;
            }
            long frames = PullBackTimer;
            PullBackAmount = (float)tanh(frames * 0.05f);
        } else {
            if (ButtonPressure < 0.3f && ++ButtonReleaseCancelTimer > 2) {
                CancelAimingWeapon();
                return;
            }
            float amount = (PullBackAmount * 3.0f + ButtonPressure) * 0.25f;
            PullBackAmount = amount;
            if (amount < 0.0f)
                PullBackAmount = 0.0f;
            else if (amount > 1.0f)
                PullBackAmount = 1.0f;
        }
        float pressure = ButtonPressure;
        const CFloatVector& roots = BuildSpline_CubicRootFinding(pressure, 0.0f, 10000.0f, true, false);
        if (roots._M_start != roots._M_finish) {
            float root = *roots._M_start;
            if (PullBackTimer > root)
                PullBackTimer = (long)root;
        }
        if (PullBackAmount > 0.95f)
            Decapitate = true;
        if (AimingTimer >= 0)
            DamageMultiplier = GetDamageMultiplierFromThingAtBuildUpTime(ParentThing, PHeroCombatDef, (float)AimingTimer / g_FramesPerSecond);
        Accuracy = 100.0f;
        if (ParentThing->HasTC(0x46)) {
            CThing* weapon = ParentThing->FindTCConst<CTCCarrying>(0x46)->GetThingInPrimarySlot();
            if (IS_THING_ALIVE(weapon) && weapon->HasTC(0x48)
                && weapon->FindTCConst<CTCWeapon>(0x48)->IsProjectileWeaponLoaded() && AimingTimer < 0) {
                AimingTimer = 0;
                PullBackTimer = 0;
                Loading = false;
            }
        }
        LoadRangedWeapon();
        FireRangedWeapon();
        LastButtonPressure = ButtonPressure;
        if (!FireQueued)
            Aiming = false;
        return;
    }
    if (IsAbleToUseRangedWeapon()) {
        CThing* weapon = ParentThing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        CTCWeapon* tc = weapon->GetTC<CTCWeapon>(0x48);
        if (tc->IsProjectileWeaponLoaded()) {
            Loading = false;
            return;
        }
        if (tc->IsAnalogueLoading()) {
            LoadRangedWeapon();
            return;
        }
    } else {
        CThing* weapon = ParentThing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(weapon) && weapon->HasTC(0x48)) {
            CTCWeapon* tc = weapon->FindTC<CTCWeapon>(0x48);
            if (tc->IsProjectileWeapon() && !tc->IsProjectileWeaponLoaded() && tc->IsAnalogueLoading()) {
                CPlayer* player = ParentThing->GetPlayer();
                if (player->GetCurrentMode() == 1)
                    player->AddMode(0x22, false);
            }
        }
    }
    CancelAimingWeapon();
}
