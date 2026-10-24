// GetPRangedWeaponFromCreature(const CThingCreatureBase& creature) -- the live projectile weapon in the creature's primary carry slot, or null
// Leaf: ?GetPRangedWeaponFromCreature@
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
enum ETCInterfaceType { TC_CARRYING = 0x46, TC_WEAPON = 0x48 };

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
    __forceinline SEntry* Find(long type) const
    {
        SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->type) ? last : it;
    }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap Components;                       // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool PeekTC(const T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (const T*)Components.Find(T::StatGetInterfaceType())->component;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing {
};

inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->Dead)
        return true;
    return false;
}

class CTCWeapon {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_WEAPON; }
    bool IsProjectileWeapon() const;                    // 0x005DAA40
};

class CTCCarrying {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_CARRYING; }
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
};

namespace {
CThing* __fastcall GetPRangedWeaponFromCreature(const CThingCreatureBase& creature)
{
    const CTCCarrying* carrying;
    if (creature.PeekTC(&carrying)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        const CTCWeapon* weapon;
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