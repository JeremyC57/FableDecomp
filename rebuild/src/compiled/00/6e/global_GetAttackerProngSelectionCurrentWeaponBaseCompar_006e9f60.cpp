// GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing& attacker, bool wantProjectile) -- (anonymous namespace) the weapon part of an attacker's prong-selection score: 5 when whether the attacker is using a projectile weapon (its unsheathing weapon type, else the projectile test of the weapon in its primary carrying slot) matches the wanted kind, else 0
// Leaf: ?GetAttackerProngSelectionCurrentWeaponBaseCompareValue@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9F60, 236 bytes (240 with int3 padding), file-static `__fastcall`
//   (ecx = attacker, dl = wantProjectile), plain `ret`, float in st0.
// Identity: Ego_r.pdb name set `?GetAttackerProngSelectionCurrentWeaponBaseCompareValue@?A0xd8b789b9@@YIMABVCThing@@_N@Z`
//   (anonymous namespace of the combat-manager TU); callers 0x006F2DA5 / 0x006F2F28 add it to
//   GetAttackerProngSelectionCompareValue 0x006E7F00.
// Callees: CInterfaceMap::LowerBound (const) @ 0x004365B0 (three inlined lookups: CTCCombat 0x30, CTCCarrying 0x46,
//   CTCWeapon 0x48), CTCCarrying::GetThingInPrimarySlot() @ 0x006A1440, CTCWeapon::IsProjectileWeapon() @ 0x005DAA40.
// Globals: 5.0f @ 0x01261D64, 0.0f @ 0x0122DEDC.
// Layout (retail): CThing ThingTypeFlags +0x28 (0x40 creature, 0x100 the weapon kind), component bits +0x20, interface
//   map +0x44, Dead +0x91 bit 0; CTCCombat UnsheathingWeaponType +0x6C (PDB +0x78: three 12-byte STL members before it).
// Shape (agent902, 3rd grade of pass 2): the combat component through the landed 0x007504E0 out-pointer
//   `__forceinline bool GetInterface(long type, T** out)` (bit test inside, `if (it == last || type < it->type) it = last;`)
//   called as `if (GetInterface(0x30, &combat))` -- the component then lands in eax (`mov eax,[eax+4] ; mov eax,[eax+6Ch]`)
//   and the carrying/weapon key addresses take retail's ecx/edx; the carrying and weapon components are looked up
//   unchecked (the creature / weapon flag guarantees them); the positive verdict is tested first
//   (`if (IsUsingProjectileWeapon(..)) return want ? 5 : 0; return want ? 0 : 5;`), which puts the not-projectile arm
//   right after the combat test as retail; the three key words share one `push ecx` slot with the same-TU LowerBound body.
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