// GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing& attacker, bool wantProjectile) -- the weapon part of an attacker's prong-selection score: 5 when whether the attacker is using a projectile weapon (its unsheathing weapon type, else the projectile test of the weapon in its primary carrying slot) matches the wanted kind, else 0
// Grade: DIFFER(236v236) -- same length and instructions except: the two result arms sit swapped (retail puts the not-projectile arm right after the combat test: `cmp eax,5 ; je TRUE` and `test al,al ; je FALSE`, ours `jne`/`jne` with the fld constants exchanged), retail reads the combat component into eax (`mov eax,[eax+4] ; mov eax,[eax+6Ch]`, ours via ecx) and forms the carrying key address in ecx (ours edx)
// Leaf: ?GetAttackerProngSelectionCurrentWeaponBaseCompareValue@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9F60, 236 bytes (240 with int3 padding), file-static `__fastcall`
//   (ecx = attacker, dl = wantProjectile), plain `ret`, float in st0.
// Identity: Ego_r.pdb name set `?GetAttackerProngSelectionCurrentWeaponBaseCompareValue@?A0xd8b789b9@@YIMABVCThing@@_N@Z`
//   (anonymous namespace of the combat-manager TU); callers 0x006F2DA5 / 0x006F2F28 add it to
//   GetAttackerProngSelectionCompareValue 0x006E7F00 (this chunk). The catalog name is the PDB name.
// Callees: CInterfaceMap::LowerBound (const) @ 0x004365B0 (three inlined GetTC lookups: CTCCombat 0x30, CTCCarrying 0x46,
//   CTCWeapon 0x48), CTCCarrying::GetThingInPrimarySlot() @ 0x006A1440, CTCWeapon::IsProjectileWeapon() @ 0x005DAA40.
// Globals: 5.0f @ 0x01261D64, 0.0f @ 0x0122DEDC.
// Layout (retail): CThing ThingTypeFlags +0x28 (0x40 creature, 0x100 the weapon kind), component bits +0x20, interface
//   map +0x44, Dead +0x91 bit 0; CTCCombat UnsheathingWeaponType +0x6C (PDB +0x78: three 12-byte STL members before it).
// Shape: the combat component is presence-tested (bit 16 of the second word, a dword test only when HasTC returns the
//   masked word, not a bool); the carrying and weapon components are looked up unchecked (the creature / weapon flag
//   guarantees them); the three key words share one `push ecx` slot only with a same-TU LowerBound body that does not
//   pass the key on; the projectile verdict is a __forceinline bool whose constant results VC threads into the two
//   `test bl,bl` tails once the type test is `if (type == 5) return true; return false;` (`return type == 5` sets al).
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
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};

static __forceinline bool IsUsingProjectileWeapon(const CThing& thing)
{
    if (thing.HasTC(0x30)) {
        long weaponType = thing.GetTC<CTCCombat>(0x30)->UnsheathingWeaponType;
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

float __fastcall GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing& attacker, bool wantProjectile)
{
    if (!IsUsingProjectileWeapon(attacker))
        return wantProjectile ? 0.0f : 5.0f;
    return wantProjectile ? 5.0f : 0.0f;
}
