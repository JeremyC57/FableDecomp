// CCreatureAction_UnsheatheItemFromInventory::GetAnimationToUse(const CThing& creature, const CThing& item) -- the item's weapon unsheathe animation, else "STANDARD_UNSHEATHE"
// Leaf: GetAnimationToUse
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F980, length 96 bytes, `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CTCWeapon::GetUnsheatheAnim() const @ 0x005DA540 (manifest label "GetSheatheAnim" is propagated; the
//   sheathe sibling 0x0069F3E0 calls 0x005DA5A0), CCharString(const CCharString&) @ 0x0099EC30,
//   CCharString(const char*, long) @ 0x0099EBF0.
// Identity: Ego_r.pdb `?GetAnimationToUse@CCreatureAction_UnsheatheItemFromInventory@@CI?AVCCharString@@ABVCThing@@0@Z`
//   (private static __fastcall: hidden result in ecx, the unused creature in edx, the item on the stack -> `ret 4`);
//   the literal 0x0125D028 is "STANDARD_UNSHEATHE".
// Layout: the local CThing model of the sheathe sibling (weapon component 0x48 = word +0x28 bit 8, map at +0x44).
// Shape: PeekTC<CTCWeapon> on the item (key in the dead item-argument slot: no frame), the hit returns the
//   weapon's unsheathe CCharString through the copy constructor, the miss builds the default literal.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCWeapon {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TCI_WEAPON; }
    const CCharString& GetUnsheatheAnim() const;    // 0x005DA540
};

class CCreatureAction_UnsheatheItemFromInventory {
    static CCharString __fastcall GetAnimationToUse(const CThing& creature, const CThing& item);
};

CCharString __fastcall CCreatureAction_UnsheatheItemFromInventory::GetAnimationToUse(const CThing& creature, const CThing& item)
{
    const const CTCWeapon* weapon;
    if (item.PeekTC(&weapon))
        return weapon->GetUnsheatheAnim();
    return CCharString("STANDARD_UNSHEATHE");
}