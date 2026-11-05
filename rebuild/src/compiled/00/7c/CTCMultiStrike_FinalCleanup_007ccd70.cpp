// CTCMultiStrike::FinalCleanup() -- find the parent's weapon (the inventory's active melee weapon, else the thing carried in its primary slot) and take the multi-strike effect off it
// Leaf: ?FinalCleanup@CTCMultiStrike@@AAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CCD70, 137 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?FinalCleanup@CTCMultiStrike@@AAEXXZ (private void()); its only references are the
//   CTCMultiStrike vtable thunks (vtable 0x0126EE0C) 0x007CCE00 (slot 14, OnKill) and 0x007CCE10 (slot 2, OnRemove) that
//   tail-jump here (the ability-TC pattern of the landed CTCWillLightning::FinalCleanup 0x007A0950). Label correction: the
//   landed 0x007CCE10 is catalogued as FinalCleanup (it is OnRemove), 0x007CCE00 as vfunc_14 (OnKill).
// Callees: the const CVectorMap LowerBound 0x004365B0 (inline const GetInterface: 0x13 CTCInventoryWeapons, 0x46
//   CTCCarrying -- interface ids from their GetInterfaceType slots), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCCarrying::GetThingInPrimarySlot() const 0x006A1440 (Ego_r.pdb), CTCMultiStrike::RemoveEffectOnWeapon(CThing*)
//   0x007CC9E0 (Ego_r.pdb).
// Layout: CTCInventoryWeapons PActiveMeleeWeapon +0x134 (Ego_r +0x14C); CThing bitset +0x20, interface map +0x44.
// Shape: the lookups go through a const parent (the const LowerBound copy); a missing weapon interface falls to the
//   carrying lookup, and both arms meet at the null test before the call.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCInventoryWeapons {                         // interface 0x13
public:
    unsigned char _pad00[0x134];
    CIntelligentPointer<CThing> PActiveMeleeWeapon; // +0x134
};
class CTCCarrying {                                 // interface 0x46
public:
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
};

class CTCMultiStrike {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    void RemoveEffectOnWeapon(CThing* weapon);      // 0x007CC9E0
private:
    void FinalCleanup();
};

void CTCMultiStrike::FinalCleanup()
{
    const CThing* parent = ParentThing;
    CThing* weapon = 0;
    const const CTCInventoryWeapons* weapons;
    const const CTCCarrying* carrying;
    if (parent->PeekTC(TCI_INVENTORY_WEAPONS, &weapons))
        weapon = weapons->PActiveMeleeWeapon.Get();
    else if (parent->PeekTC(TCI_CARRYING, &carrying))
        weapon = carrying->GetThingInPrimarySlot();
    if (weapon)
        RemoveEffectOnWeapon(weapon);
}