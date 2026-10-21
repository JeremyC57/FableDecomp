#include <stdio.h>
#include <string.h>
// CTCHeroRangedAttacks::CancelAimingWeapon() -- abandon the aim: reset timers, pressures and cycle flags, recompute damage and
// accuracy for zero build-up, drop the projectile carried in the weapon's carry slot, unload a non-analogue projectile weapon
// (resetting its animation), stop the bow draw-back sound
// Shape: GetInterface stores the component through the out-pointer as ONE ternary
//   `*ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;` (the landed SetupDressingAtLevel spelling),
//   and the sound player is fetched with GetInterface (not HasTC + FindTC): VC7.1 then loads the component into ecx before the
//   BowDrawbackSoundID test as retail; the CVectorMap LowerBound instantiation is defined in this TU (noinline).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789FF0, 449 bytes (+ int3 padding), `ret`.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10)) {
        if (BowDrawbackSoundID)
            sound->StopSample(BowDrawbackSoundID);
    }
    BowDrawbackSoundID = 0;
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static int g_destroyed, g_kills, g_killImm, g_removed, g_removedSlot, g_removedDrop, g_played, g_loaded, g_loadedArg, g_stopped;
static unsigned long g_stopId; static void* g_stopThis; static CThing* g_killThis; static CThing* g_loadedAmmo;
static CThing* g_primary; static CThing* g_ammoThing; static long g_carrySlot; static bool g_projectile, g_analogue;
static float g_dmgBuild = -1.0f, g_accBuild = -1.0f; static long g_defRefsAtCall;
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { g_destroyed++; }
template<class T> CDefPointer<T>::~CDefPointer() { if (Object && --Object->RefCount == 0) ((CDefPointeeBase*)Object)->Destroy(); }
float __fastcall GetDamageMultiplierFromThingAtBuildUpTime(CThing* thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime) { g_dmgBuild = buildUpTime; g_defRefsAtCall = def.Object->RefCount; (void)thing; return 2.5f; }
float __fastcall GetAccuracyPercentageFromThingAtBuildUpTime(CThing* thing, CDefPointer<const CHeroCombatDef> def, float buildUpTime) { g_accBuild = buildUpTime; (void)thing; (void)def; return 0.75f; }
void CThing::Kill(bool immediately) { g_kills++; g_killImm = immediately; g_killThis = this; }
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_primary; }
CThing* CTCCarrying::RemoveThingInCarrySlot(long slot, bool drop) { g_removed++; g_removedSlot = slot; g_removedDrop = drop; return g_ammoThing; }
long CTCWeapon::GetAmmoCarrySlot() { return g_carrySlot; }
bool CTCWeapon::IsProjectileWeapon() { return g_projectile; }
bool CTCWeapon::IsAnalogueLoading() { return g_analogue; }
void CTCWeapon::SetProjectileWeaponLoaded(bool loaded, CThing* ammo) { g_loaded++; g_loadedArg = loaded; g_loadedAmmo = ammo; }
void CTCAnimatingObject::PlayDefaultAnimation() { g_played++; }
void CTCSoundPlayer::StopSample(unsigned long sampleID) { g_stopped++; g_stopId = sampleID; g_stopThis = this; }
static void Reset() { g_destroyed = g_kills = g_killImm = g_removed = g_removedSlot = g_removedDrop = g_played = g_loaded = g_loadedArg = g_stopped = 0; g_stopId = 0; g_stopThis = 0; g_killThis = 0; g_loadedAmmo = (CThing*)1; }
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
static unsigned char g_parentBuf[0x100], g_weaponBuf[0x100], g_ammoBuf[0x100], g_rangedBuf[sizeof(CTCHeroRangedAttacks)];
int main()
{
    CThing* parent = (CThing*)g_parentBuf; CThing* weapon = (CThing*)g_weaponBuf; CThing* ammo = (CThing*)g_ammoBuf;
    static CTCCarrying carrying; static CTCSoundPlayer sound; static CTCWeapon weaponTC; static CTCAnimatingObject anim;
    CInterfaceMap::SEntry pe[] = { { 0x10, (CTCBase*)&sound }, { 0x46, (CTCBase*)&carrying } };
    CInterfaceMap::SEntry we[] = { { 0x48, (CTCBase*)&weaponTC }, { 0x83, (CTCBase*)&anim } };
    SetInterfaces(parent, pe, 2); SetInterfaces(weapon, we, 2);
    static CHeroCombatDef def; def.RefCount = 1;
    CTCHeroRangedAttacks* r = (CTCHeroRangedAttacks*)g_rangedBuf;
    r->ParentThing = parent; r->PHeroCombatDef.Object = &def;
    // full cancel: projectile non-analogue weapon with ammo in carry slot 3, bow sound playing
    r->AimingTimer = 5; r->PullBackTimer = 6; r->ButtonPressure = 0.9f; r->LastButtonPressure = 0.8f; r->FireCycleInProgress = r->Aiming = r->FireQueued = r->Loading = true;
    r->ButtonReleaseCancelTimer = 9; r->PullBackAmount = 0.5f; r->BowDrawbackSoundID = 77;
    g_primary = weapon; g_ammoThing = ammo; g_carrySlot = 3; g_projectile = true; g_analogue = false; Reset();
    r->CancelAimingWeapon();
    CHECK(r->AimingTimer == -1 && r->PullBackTimer == -1 && r->ButtonPressure == 0.01f && r->LastButtonPressure == 0.01f);
    CHECK(!r->FireCycleInProgress && !r->Aiming && !r->FireQueued && !r->Loading && r->ButtonReleaseCancelTimer == 0 && r->PullBackAmount == 0.0f);
    CHECK(r->DamageMultiplier == 2.5f && r->Accuracy == 0.75f && g_dmgBuild == 0.0f && g_accBuild == 0.0f && g_defRefsAtCall == 2 && def.RefCount == 1 && g_destroyed == 0);
    CHECK(g_removed == 1 && g_removedSlot == 3 && g_removedDrop == 0 && g_kills == 1 && g_killImm == 1 && g_killThis == ammo);
    CHECK(g_played == 1 && g_loaded == 1 && g_loadedArg == 0 && g_loadedAmmo == 0);
    CHECK(g_stopped == 1 && g_stopId == 77 && g_stopThis == &sound && r->BowDrawbackSoundID == 0);
    // no sound id; analogue weapon (no unload); negative carry slot; no animating object
    SetInterfaces(weapon, we, 1); g_carrySlot = -1; g_analogue = true; r->BowDrawbackSoundID = 0; Reset();
    r->CancelAimingWeapon();
    CHECK(g_removed == 0 && g_kills == 0 && g_played == 0 && g_loaded == 0 && g_stopped == 0);
    // non-analogue, but no animating object: unload without animation; dead ammo is not killed
    g_analogue = false; g_carrySlot = 1; ammo->m_flags91 = 1; Reset();
    r->CancelAimingWeapon();
    CHECK(g_removed == 1 && g_kills == 0 && g_played == 0 && g_loaded == 1);
    // dead weapon: nothing weapon-related
    weapon->m_flags91 = 1; Reset(); r->BowDrawbackSoundID = 12;
    r->CancelAimingWeapon();
    CHECK(g_removed == 0 && g_loaded == 0 && g_stopped == 1 && g_stopId == 12 && r->BowDrawbackSoundID == 0);
    // no carrying, no sound player: only the field resets
    SetInterfaces(parent, pe, 0); r->BowDrawbackSoundID = 5; r->Aiming = true; Reset();
    r->CancelAimingWeapon();
    CHECK(g_stopped == 0 && g_removed == 0 && !r->Aiming && r->BowDrawbackSoundID == 0);
    if (fails == 0) printf("00789ff0_TEST PASS\n"); else printf("00789ff0_TEST FAIL\n");
    return fails;
}