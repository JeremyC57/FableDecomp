// GetPRangedWeaponFromCreature(const CThingCreatureBase& creature) -- the live projectile weapon in the creature's primary carry slot, or null
// Leaf: ?GetPRangedWeaponFromCreature@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00905600, 138 bytes, __fastcall (creature in ecx), returns CThing*.
// Identity: Ego_r.pdb `?GetPRangedWeaponFromCreature@?A0x3295c2ae@@YIPAVCThing@@ABVCThingCreatureBase@@@Z` -- a
//   file-local (anonymous namespace) helper of the balverine-combat TU (NBalverineCombat::CanLungeThisFrame 0x00905550
//   sits right above it); its three callers are in the same TU.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const 0x004365B0,
//   CTCCarrying::GetThingInPrimarySlot() const 0x006A1440, CTCWeapon::IsProjectileWeapon() const 0x005DAA40.
// Layout (retail, as the landed CTCCarrying::IsThingCarryingRangedWeapon 0x006A1620): the component-presence bitset at
//   +0x20 (carrying 0x46 = byte +0x28 bit 6, weapon 0x48 = dword +0x28 bit 8), the sorted {kind, component} vector
//   map at +0x44, the Dead bit = bit 0 of +0x91.
// Shape: the twin of 0x006A1620 returning the item instead of the weapon's bool: PeekTC<T> (HasTC + Find with a
//   converted key temporary sharing the one `push ecx` frame word), IsThingAlive, and the three-way && whose
//   success returns the item kept in edi; every miss shares the `xor eax,eax` tail.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing {
};

inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->IsDead())
        return true;
    return false;
}

class CTCWeapon {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TCI_WEAPON; }
    bool IsProjectileWeapon() const;                    // 0x005DAA40
};

class CTCCarrying {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TCI_CARRYING; }
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
};

namespace {
CThing* __fastcall GetPRangedWeaponFromCreature(const CThingCreatureBase& creature)
{
    const const CTCCarrying* carrying;
    if (creature.PeekTC(&carrying)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        const const CTCWeapon* weapon;
        if (IsThingAlive(item) && item->PeekTC(&weapon) && weapon->IsProjectileWeapon())
            return item;
    }
    return 0;
}
}

// The retail callers (same TU); keeps the file-local function emitted.
CThing* __fastcall CallGetPRangedWeaponFromCreature(const CThingCreatureBase& creature)
{
    return GetPRangedWeaponFromCreature(creature);
}