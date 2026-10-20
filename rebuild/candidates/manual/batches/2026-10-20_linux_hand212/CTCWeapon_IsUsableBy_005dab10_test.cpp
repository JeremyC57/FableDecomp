// Behaviour test for 005dab10: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCWeapon::IsUsableBy(const CThing& thing) const -- only creatures may use a weapon; class-1 things, a suppressed game, things without hero experience and weapon classes 0/1/3 always may; class 2 needs berserk or the experience requirements
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// (engine/CTCWeapon.h, self-contained copy without its 32-bit size asserts)
struct CDefPointeeBase;
struct CTCWeapon {
    void*            __vftable;                              // +0x00
    unsigned char    _pad_0x04[0x8];                         // +0x04
    CDefPointeeBase* WeaponDef_Object;                       // +0x0c
    bool             ProjectileWeaponLoaded;                 // +0x10
    long             AimingTime;                             // +0x14
    bool             AimingTimeOverride;                     // +0x18
    long             NumThingsKilled;                        // +0x1c
    long             TimeWielded;                            // +0x20
    bool             Wielded;                                // +0x24
};

struct CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long& type) const;   // 0x004365B0 (the const instantiation)
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingClass;                   // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;               // +0x6c bit 3: creature
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCHeroExperience;                       // interface 0x68
struct CExperienceRequirements {                // the +0x13c sub-object of the creature's +0x1e8 record
    bool IsMetBy(const CTCHeroExperience* experience) const;   // 0x00851B50
};
struct CCreatureRecord1E8 {                     // unidentified
    unsigned char           _pad00[0x13c];
    CExperienceRequirements Requirements;       // +0x13c
};
struct CThingCreatureBase : CThing {
    const CCreatureRecord1E8* GetRecord1E8() const;   // 0x006A98D0
};

struct CGameSubsystem {                         // [0x013B8A1C]+0x1c (unidentified)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSuppressed();                // slot 9 (+0x24)
};

enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1, WEAPON_CLASS_2 = 2, WEAPON_CLASS_3 = 3 };

struct CTCWeapon_Getters : CTCWeapon {
    CGameSubsystem* GetGameSubsystem();         // 0x00686D60 (CTCBase)
    EWeaponClass GetWeaponClass() const;        // 0x005DA7F0
    bool IsUsableBy(const CThing& thing) const;
};

bool CTCWeapon_Getters::IsUsableBy(const CThing& thing) const
{
    if (thing.ThingTypeFlags & 8) {
        if (thing.ThingClass == 1 || ((CTCWeapon_Getters*)this)->GetGameSubsystem()->IsSuppressed())
            return true;
        const CCreatureRecord1E8* record = static_cast<const CThingCreatureBase&>(thing).GetRecord1E8();
        const CTCHeroExperience* experience;
        if (!thing.GetInterface(&experience, 0x68))
            return true;
        switch (GetWeaponClass()) {
        case WEAPON_CLASS_NONE:
        case WEAPON_CLASS_1:
        case WEAPON_CLASS_3:
            return true;
        case WEAPON_CLASS_2:
            return thing.HasTC(0x84) || record->Requirements.IsMetBy(experience);
        }
    }
    return false;
}

// ---- stubs and driver ----
void CThing::v00() {}
static bool g_suppressed;
void CGameSubsystem::v00() {} void CGameSubsystem::v01() {} void CGameSubsystem::v02() {} void CGameSubsystem::v03() {}
void CGameSubsystem::v04() {} void CGameSubsystem::v05() {} void CGameSubsystem::v06() {} void CGameSubsystem::v07() {}
void CGameSubsystem::v08() {}
bool CGameSubsystem::IsSuppressed() { return g_suppressed; }
static CGameSubsystem g_subsystem;
CGameSubsystem* CTCWeapon_Getters::GetGameSubsystem() { return &g_subsystem; }
static EWeaponClass g_class;
EWeaponClass CTCWeapon_Getters::GetWeaponClass() const { return g_class; }
static CCreatureRecord1E8 g_record;
static const CThing* g_recordOf;
const CCreatureRecord1E8* CThingCreatureBase::GetRecord1E8() const { g_recordOf = this; return &g_record; }
static bool g_met; static const CTCHeroExperience* g_checked; static int g_checks;
bool CExperienceRequirements::IsMetBy(const CTCHeroExperience* experience) const
{
    if (this != &g_record.Requirements) return false;
    g_checked = experience; ++g_checks; return g_met;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type) const
{
    const SEntry* it = first;
    while (it != last && it->type < type) ++it;
    return it;
}

static CThingCreatureBase g_thing;
static CInterfaceMap::SEntry g_entries[2];
static int g_experience;

static int Expect(const CTCWeapon_Getters& w, bool want, const char* what)
{
    if (w.IsUsableBy(g_thing) != want) { std::printf("005dab10_TEST FAIL (%s)\n", what); return 1; }
    return 0;
}

int main()
{
    CTCWeapon_Getters w; std::memset(&w, 0, sizeof(w));
    g_entries[0].type = 0x68; g_entries[0].component = (CTCBase*)&g_experience;
    g_entries[1].type = 0x84; g_entries[1].component = 0;
    g_thing.m_interfaces.first = g_entries; g_thing.m_interfaces.last = g_entries + 2;
    int fails = 0;
    // not a creature: never usable
    g_thing.ThingTypeFlags = 0; g_class = WEAPON_CLASS_2;
    fails += Expect(w, false, "non-creature");
    g_thing.ThingTypeFlags = 8;
    // class-1 things and a suppressed game: always usable
    g_thing.ThingClass = 1; fails += Expect(w, true, "class 1");
    g_thing.ThingClass = 0; g_suppressed = true; fails += Expect(w, true, "suppressed");
    g_suppressed = false;
    // no hero experience: usable
    fails += Expect(w, true, "no experience");
    g_thing.m_presentBits[3] = 1UL << 8;   // interface 0x68
    // weapon classes 0, 1, 3 usable; an unknown class is not
    g_class = WEAPON_CLASS_NONE; fails += Expect(w, true, "class 0");
    g_class = WEAPON_CLASS_1; fails += Expect(w, true, "class 1 weapon");
    g_class = WEAPON_CLASS_3; fails += Expect(w, true, "class 3");
    g_class = (EWeaponClass)7; fails += Expect(w, false, "unknown class");
    if (g_checks) { std::printf("005dab10_TEST FAIL (checked early)\n"); return 1; }
    // class 2: the experience requirements decide, unless the thing is berserk
    g_class = WEAPON_CLASS_2;
    g_met = false; fails += Expect(w, false, "class 2 unmet");
    if (g_checks != 1 || g_checked != (const CTCHeroExperience*)&g_experience || g_recordOf != &g_thing)
        { std::printf("005dab10_TEST FAIL (requirement args)\n"); return 1; }
    g_met = true; fails += Expect(w, true, "class 2 met");
    g_met = false; g_thing.m_presentBits[4] = 1UL << 4;   // interface 0x84 (berserk)
    fails += Expect(w, true, "class 2 berserk");
    if (g_checks != 2) { std::printf("005dab10_TEST FAIL (berserk short-circuit)\n"); return 1; }
    if (fails) return 1;
    std::printf("005dab10_TEST PASS\n");
    return 0;
}
