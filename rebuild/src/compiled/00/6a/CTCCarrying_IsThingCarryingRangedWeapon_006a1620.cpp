// CTCCarrying::IsThingCarryingRangedWeapon(const CThing& thing) -- is the live weapon in the thing's primary carry slot a ranged (projectile) weapon
// Leaf: IsThingCarryingRangedWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1620, length 127 bytes (+ int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CTCCarrying::GetThingInPrimarySlot() const @ 0x006A1440 (landed), CTCWeapon::IsProjectileWeapon() const @ 0x005DAA40.
// Identity: Ego_r.pdb `?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z` (static, __fastcall: the thing in ecx); the three
//   IsThingCarrying* statics sit in PDB order at 0x006A1540 / 0x006A15A0 / 0x006A1620 (the catalog labels both
//   0x006A15A0 and 0x006A1620 IsThingCarryingMeleeWeapon; 0x006A1620 calls IsProjectileWeapon).
// Layout (retail, local model as the landed CThingFilter sources): the component-presence bitset at +0x20 (carrying
//   0x46 = byte +0x28 bit 6, weapon 0x48 = word +0x28 bit 8), the sorted {kind, component} vector map at +0x44, the
//   Dead bit = bit 0 of +0x91 (landed IsThingAlive 0x006961F0).
// Shape: the PDB's PeekTC<T>(const T**) template: HasTC on T::StatGetInterfaceType(), then the map's Find(long)
//   whose LowerBound key is a long->ETCInterfaceType conversion TEMPORARY -- both lookups then share the one
//   `push ecx` frame word (two named key locals take two words, `sub esp,8`) while the key test keeps the
//   immediate (`cmp [eax],46h ; jle`). esi is pushed before the first test because both lookups use it; the
//   weapon query's own bool is returned as is, the misses share the `xor al,al` tail.
class CTCBase;
enum ETCInterfaceType { TC_CARRYING = 0x46, TC_WEAPON = 0x48 };

namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
    __forceinline const SEntry* Find(long type) const
    {
        const SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->first) ? last : it;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
            *pp = (const T*)Components.Find(T::StatGetInterfaceType())->second;
            return true;
        }
        return false;
    }
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
    static bool __fastcall IsThingCarryingRangedWeapon(const CThing& thing);
};

bool __fastcall CTCCarrying::IsThingCarryingRangedWeapon(const CThing& thing)
{
    const CTCCarrying* carrying;
    if (thing.PeekTC(&carrying)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        const CTCWeapon* weapon;
        if (IsThingAlive(item) && item->PeekTC(&weapon))
            return weapon->IsProjectileWeapon();
    }
    return false;
}