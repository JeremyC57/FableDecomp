// CCreatureAction_SheatheItemToInventory::GetAnimationToUse(const CThing& creature) -- the sheathe animation of the live weapon in the creature's primary carry slot, else "STANDARD_SHEATHE"
// Leaf: GetAnimationToUse
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F3E0, length 156 bytes (+ int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CTCCarrying::GetThingInPrimarySlot() const @ 0x006A1440 (landed), CTCWeapon::GetSheatheAnim() const @ 0x005DA5A0,
//   CCharString::CCharString(const CCharString&) @ 0x0099EC30, CCharString::CCharString(const char*, long) @ 0x0099EBF0.
// Identity: Ego_r.pdb `?GetAnimationToUse@CCreatureAction_SheatheItemToInventory@@CI?AVCCharString@@ABVCThing@@@Z`
//   (private static __fastcall: the hidden CCharString result in ecx, the creature in edx); the literal
//   0x0125D014 is "STANDARD_SHEATHE".
// Layout (retail, local model as the landed CTCCarrying::IsThingCarrying* rows 0x006A1540..0x006A1620): component
//   bitset at +0x20 (carrying 0x46, weapon 0x48), {kind, component} vector map at +0x44, Dead bit +0x91 bit 0.
// Shape: the IsThingCarryingMeleeWeapon body (PeekTC<T> with the long->ETCInterfaceType conversion temporary as
//   the one shared key word) returning `weapon->GetSheatheAnim()` through the copy constructor into the hidden
//   result; every miss falls to the one `CCharString("STANDARD_SHEATHE")` construction (`push -1 ; push offset`).
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->IsDead())
        return true;
    return false;
}

class CTCWeapon {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TCI_WEAPON; }
    const CCharString& GetSheatheAnim() const;      // 0x005DA5A0
};

class CTCCarrying {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TCI_CARRYING; }
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
};

class CCreatureAction_SheatheItemToInventory {
    static CCharString __fastcall GetAnimationToUse(const CThing& creature);
};

CCharString __fastcall CCreatureAction_SheatheItemToInventory::GetAnimationToUse(const CThing& creature)
{
    const const CTCCarrying* carrying;
    if (creature.PeekTC(&carrying)) {
        CThing* item = carrying->GetThingInPrimarySlot();
        const const CTCWeapon* weapon;
        if (IsThingAlive(item) && item->PeekTC(&weapon))
            return weapon->GetSheatheAnim();
    }
    return CCharString("STANDARD_SHEATHE");
}