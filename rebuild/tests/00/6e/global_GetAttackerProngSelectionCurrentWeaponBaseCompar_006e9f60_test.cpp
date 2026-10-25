// Behaviour test for 0x006E9F60 (source body copied from src/006e9f60.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the const LowerBound 0x004365B0 (the STLport lower_bound over the sorted {type, component} vector):
// VC7.1 then sees the key's address go no further, so the three inlined lookups can share one key word.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* f = first;
    int count = last - first;
    while (count > 0) {
        int half = count >> 1;
        SEntry* middle = f + half;
        if (middle->type < *type) {
            f = middle + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return f;
}
#pragma auto_inline(on)

class CThing;
class CTCCombat {
public:
    unsigned char _pad00[0x6c];
    long UnsheathingWeaponType;                             // +0x6C (EUnsheatheWeaponType; 5 = projectile)
};
class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;                  // 0x006A1440
};
class CTCWeapon {
public:
    bool IsProjectileWeapon() const;                        // 0x005DAA40
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[2];                         // +0x20
    unsigned long ThingTypeFlags;                           // +0x28
    unsigned char _pad2c[0x44 - 0x2c];
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

static __forceinline bool IsUsingProjectileWeapon(const CThing& thing)
{
    CTCCombat* combat;
    if (thing.GetInterface(0x30, &combat)) {
        long weaponType = combat->UnsheathingWeaponType;
        if (weaponType != 0) {
            if (weaponType == 5)
                return true;
            return false;
        }
    }
    if (thing.ThingTypeFlags & 0x40) {
        const CThing* weapon = thing.GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (weapon && !weapon->IsDead() && (weapon->ThingTypeFlags & 0x100))
            return weapon->GetTC<CTCWeapon>(0x48)->IsProjectileWeapon();
    }
    return false;
}

namespace {
float __fastcall GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing& attacker, bool wantProjectile)
{
    if (IsUsingProjectileWeapon(attacker))
        return wantProjectile ? 5.0f : 0.0f;
    return wantProjectile ? 0.0f : 5.0f;
}
}

// Keeps the anonymous-namespace function alive in the object (retail reaches it from GetAttackerProngSelectionCompareValue's callers).
float (__fastcall* g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue)(const CThing&, bool) = &GetAttackerProngSelectionCurrentWeaponBaseCompareValue;

// ---- test harness ----
static CThing* g_primary = 0;
static bool g_projectile = false;
CThing* CTCCarrying::GetThingInPrimarySlot() const { return g_primary; }
bool CTCWeapon::IsProjectileWeapon() const { return g_projectile; }
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    static CTCCombat combat; static CTCCarrying carrying; static CTCWeapon weaponComp;
    static CThing attacker, weapon;
    memset(&attacker, 0, sizeof attacker); memset(&weapon, 0, sizeof weapon); memset(&combat, 0, sizeof combat);
    CInterfaceMap::SEntry ae[2]; ae[0].type = 0x30; ae[0].component = (CTCBase*)&combat; ae[1].type = 0x46; ae[1].component = (CTCBase*)&carrying;
    CInterfaceMap::SEntry we[1]; we[0].type = 0x48; we[0].component = (CTCBase*)&weaponComp;
    attacker.m_interfaces.first = ae; attacker.m_interfaces.last = ae + 2;
    weapon.m_interfaces.first = we; weapon.m_interfaces.last = we + 1;
    int ok = 1;
    // 1: combat component says projectile (5)
    attacker.m_presentBits[1] = 1UL << 16; combat.UnsheathingWeaponType = 5;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, true), 5.0f)) ok = 0;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, false), 0.0f)) ok = 0;
    // 2: combat says a melee weapon (3)
    combat.UnsheathingWeaponType = 3;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, true), 0.0f)) ok = 0;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, false), 5.0f)) ok = 0;
    // 3: no unsheathing type: creature with a projectile weapon in the primary slot
    combat.UnsheathingWeaponType = 0; attacker.ThingTypeFlags = 0x40;
    weapon.ThingTypeFlags = 0x100; g_primary = &weapon; g_projectile = true;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, true), 5.0f)) ok = 0;
    weapon.Dead = 1;                                  // a dead weapon does not count
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, true), 0.0f)) ok = 0;
    weapon.Dead = 0; g_projectile = false;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, false), 5.0f)) ok = 0;
    // 4: no combat component, not a creature
    attacker.m_presentBits[1] = 0; attacker.ThingTypeFlags = 0; g_projectile = true;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, true), 0.0f)) ok = 0;
    if (!Near(g_pfnGetAttackerProngSelectionCurrentWeaponBaseCompareValue(attacker, false), 5.0f)) ok = 0;
    printf("006e9f60_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}