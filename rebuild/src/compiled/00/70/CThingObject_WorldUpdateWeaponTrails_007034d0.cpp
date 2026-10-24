// CThingObject::WorldUpdateWeaponTrails() -- update the object's weapon trails and refresh its graphic appearance, unless it is a melee weapon that nobody is carrying
// Leaf: ?WorldUpdateWeaponTrails@CThingObject@@
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

class CInterfaceMap {                                     // CVectorMap<ETCInterfaceType, CTCBase*>: sorted {type, component} vector
public:
    struct SEntry {
        long type;
        CTCBase* component;
    };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const long& type);                 // 0x0040F020
    const SEntry* LowerBound(const long& type) const;     // 0x004365B0
};

// Stand-in body for the non-const lower bound 0x0040F020 (a plain binary search, as landed in 0x00838BC0), compiled
// out of line in this TU so VC sees that the lookup leaves the thing's interface bitset alone (retail keeps the bitset
// word in edi across the call).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < type) {
            it += half + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

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
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline const CTCBase* PeekTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
    void UpdateGraphicAppearance(long reason);            // 0x004C7590
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
        UpdateGraphicAppearance(4);
    }
}