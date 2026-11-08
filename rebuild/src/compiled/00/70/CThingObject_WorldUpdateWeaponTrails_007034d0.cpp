// CThingObject::WorldUpdateWeaponTrails() -- update the object's weapon trails and refresh its graphic appearance, unless it is a melee weapon that nobody is carrying
// Leaf: ?WorldUpdateWeaponTrails@CThingObject@@IAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@ABJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@ABJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 31 (family srtf): 0x004C7590 is ?SetAsRenderedThisFrame@CThing@@QAEXK@Z (Ego_r.pdb block order at 0x004C9150 + FableWin.pdb; was the
//   invented ?UpdateGraphicAppearance@CThing@@QAEXJ@Z)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007034D0, 189 bytes, plain `ret`.
// Label correction: catalog "CRenderTarget_UpdateWeaponTrails" is propagated; the only caller is
//   CThingObject::WorldUpdate (0x007041D7) and Ego_r.pdb has `?WorldUpdateWeaponTrails@CThingObject@@IAEXXZ`.
// Callees: CVectorMap LowerBound 0x0040F020 (non-const, the trail) and 0x004365B0 (const, weapon and carryable),
//   CTCWeapon::IsMeleeWeapon 0x005DAA00 (landed), CTCWeaponTrail::UpdateWeaponTrails 0x0076EFA0,
//   CThing::UpdateGraphicAppearance(long) 0x004C7590.
// Layout: interface types from each class's GetInterfaceType: CTCCarryable 0x47, CTCWeapon 0x48, CTCWeaponTrail 0x4C
//   (bits 7/8/12 of the bitset word +0x28); CTCCarryable::PCarryingTC +0x10.
// Shape: HasTC is `if (bits & mask) return true; return false;` (a `!= 0` return gives `shr ; test al,1`). Retail keeps
//   the bitset word in edi ACROSS the non-const LowerBound call: that needs the lookup's body visible in the TU, so
//   0x0040F020 is given a same-TU noinline STAND-IN body (a plain binary search, as landed in 0x00838BC0) under
//   `#pragma auto_inline(off)` -- the accepted stand-in pattern of GOTCHAS (chunks 205-206).
#include "fable_compiler.h"
class CTCBase;
class CTCCarrying;

class CTCWeaponTrail {                                    // interface 0x4c
public:
    void UpdateWeaponTrails();                            // 0x0076EFA0
};

class CTCWeapon {                                         // interface 0x48
public:
    bool IsMeleeWeapon() const;                           // 0x005DAA00 (landed)
};

class CTCCarryable {                                      // interface 0x47; Ego_r.pdb (36 bytes)
public:
    unsigned char _pad00[0x10];
    CTCCarrying* PCarryingTC;                             // +0x10
};

enum ETCInterfaceType { TC_CARRYABLE = 0x47, TC_WEAPON = 0x48, TC_WEAPON_TRAIL = 0x4c };

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentInterfaces[9];                   // +0x20 bitset of interface types
    CInterfaceMap Interfaces;                             // +0x44
    bool HasTC(long type) const
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)(key));
        if (it == map->end() || type < it->first)
            it = map->end();
        return it->second;
    }
    FABLE_FORCEINLINE const CTCBase* PeekTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)(key));
        if (it == map->end() || type < it->first)
            it = map->end();
        return it->second;
    }
    void SetAsRenderedThisFrame(unsigned long reasons);         // 0x004C7590
};

class CThingObject : public CThing {
protected:
    void WorldUpdateWeaponTrails();
};

void CThingObject::WorldUpdateWeaponTrails()
{
    if (HasTC(TC_WEAPON_TRAIL)) {
        CTCWeaponTrail* trail = (CTCWeaponTrail*)GetTC(TC_WEAPON_TRAIL);
        if (HasTC(TC_WEAPON) && ((const CTCWeapon*)PeekTC(TC_WEAPON))->IsMeleeWeapon() && HasTC(TC_CARRYABLE) &&
            ((const CTCCarryable*)PeekTC(TC_CARRYABLE))->PCarryingTC == 0)
            return;
        trail->UpdateWeaponTrails();
        SetAsRenderedThisFrame(4);
    }
}