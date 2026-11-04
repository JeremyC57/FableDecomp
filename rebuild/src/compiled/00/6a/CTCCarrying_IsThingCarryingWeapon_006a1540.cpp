// CTCCarrying::IsThingCarryingWeapon(const CThing& thing) -- does the thing's carrying component hold a live weapon (a thing with the weapon component 0x48) in its primary slot
// Leaf: IsThingCarryingWeapon
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1540, length 83 bytes (+ int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CTCCarrying::GetThingInPrimarySlot() const @ 0x006A1440 (landed).
// Identity: Ego_r.pdb `?IsThingCarryingWeapon@CTCCarrying@@SI_NABVCThing@@@Z` (static, __fastcall: the thing in ecx);
//   its siblings IsThingCarryingMeleeWeapon / IsThingCarryingRangedWeapon follow at 0x006A15A0 / 0x006A1620.
// Layout (retail, local model as the landed CThingFilter sources): the component-presence bitset at +0x20
//   (kind k = word k>>5, bit k&31: carrying 0x46 = byte +0x28 bit 6, weapon 0x48 = word +0x28 bit 8), the sorted
//   {kind, component} vector map at +0x44 {first, last}, the Dead bit = bit 0 of +0x91 (landed IsThingAlive 0x006961F0).
// Shape: HasTC + the ternary GetTC (key homed in the `push ecx` frame word, the component loaded straight into
//   ecx for the call); the item test is the inline IsThingAlive (null, dead bit) and the weapon HasTC, with the
//   shared `xor al,al` false tail.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap Components;                       // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->Dead)
        return true;
    return false;
}

class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
    static bool __fastcall IsThingCarryingWeapon(const CThing& thing);
};

enum { TC_CARRYING = 0x46, TC_WEAPON = 0x48 };

bool __fastcall CTCCarrying::IsThingCarryingWeapon(const CThing& thing)
{
    if (thing.HasTC(TC_CARRYING)) {
        CThing* item = thing.GetTC<CTCCarrying>(TC_CARRYING)->GetThingInPrimarySlot();
        if (IsThingAlive(item) && item->HasTC(TC_WEAPON))
            return true;
    }
    return false;
}