// CTCCarrying::RemoveThingFromPrimarySlot(bool) -- take the thing out of the primary carry slot (weapons are dropped, other things removed), notify its components, and remove a following melee weapon left in the slot; returns the removed thing
// Leaf: RemoveThingFromPrimarySlot
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail 0x006A1E00, 384 bytes, `ret 4` (the argument is unused).
// Callees: CTCCarrying's primary-slot empty test 0x006A1270 (labelled CThing_Array_GetFirstPhysicalSwitch),
// GetThingInPrimarySlot 0x006A1440, DropWeapon(CThing*, CTCCarrying*, CThing*) 0x00845CE0 (`__fastcall`),
// CTCCarrying::RemoveThing(CThing*) 0x006A19B0, CTCCarrying::IsCarryingThing(const CThing&) const 0x006A1060, CVectorMap::LowerBound
// 0x0040F020 (interfaces 0x1A, 0x48 weapon, 0x83 animating object) and const 0x004365B0, the 0x1A component's hook
// 0x005D8E50, CTCWeapon::IsProjectileWeapon 0x005DAA40 / SetProjectileWeaponLoaded(bool, CThing*) 0x005DAC50 /
// IsMeleeWeapon 0x005DAA70, CTCAnimatingObject::PlayDefaultAnimation 0x007F8F30.
// Layout: CTCBase ParentThing +4; CThing bitset +0x20, map +0x44, dead flag +0x91; the 0x1A component's armed pointer +0x10.
// Shape: the whole body sits in `if (!IsPrimarySlotFree()) { ... return thing; } return 0;` (an early `return 0` is laid
// out first); HasTC + ternary GetTC lookups sharing one key slot; `this` is homed in the frame for the final RemoveThing.
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
    template <class T> FABLE_FORCEINLINE T* GetTCConst(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
struct CTCInterface1A {
    unsigned char _pad00[0x10];
    void*         Armed;                                // +0x10
};
class CTCWeapon { public:
    void SetProjectileWeaponLoaded(bool loaded, CThing* ammo);   // 0x005DAC50
    bool IsProjectile() const;                               // 0x005DAA70; NF299: PDB name, was IsMeleeWeapon
public:
    bool IsProjectileWeapon() const;   // 0x005DAA40
};
class CTCAnimatingObject { public:
    void PlayDefaultAnimation();                        // 0x007F8F30
};
class CTCCarrying;
class CTCCarrying { public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    void RemoveThing(CThing& thing);                    // 0x006A19B0   // agentNF19: ?RemoveThing@CTCCarrying@@QAEXAAVCThing@@@Z (Ego_r/FableWin: by reference)
    // NF122 (2026-11-20; coordinator, NF121 recipe): Ego_r.pdb / FableWin.pdb ?IsCarryingThing@CTCCarrying@@QBE_NABVCThing@@@Z, the
    //   spelling 0x006A1060's self row and its other 17 landed callers bind; was IsCarryingThing(CThing*) non-const.
    bool IsCarryingThing(const CThing& thing) const;    // 0x006A1060
    CThing* RemoveThingFromPrimarySlot(bool unused);
public:
    bool IsPrimarySlotFree() const;   // 0x006A1270
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005D8E50 ?ReturnToInventory@CTCInventoryItem@@QAEXXZ (was ?OnUseFailed@CTCInterface1A@@QAEXXZ)
class CTCInventoryItem { public: void ReturnToInventory(); };
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00845CE0 ?DropWeapon@CCreatureAction_DropWeapon@@SIXAAVCThing@@AAVCTCCarrying@@0@Z (was ?DropWeapon@@YIXPAVCThing@@PAVCTCCarrying@@0@Z)
class CCreatureAction_DropWeapon { public: static void __fastcall DropWeapon(CThing&, CTCCarrying&, CThing&); };
CThing* CTCCarrying::RemoveThingFromPrimarySlot(bool unused)
{
    if (!IsPrimarySlotFree()) {
        CThing* thing = GetThingInPrimarySlot();
        if (thing->HasTC(0x48))
            CCreatureAction_DropWeapon::DropWeapon(*(thing), *(this), *(ParentThing));
        else
            RemoveThing(*thing);
        if (!IsCarryingThing(*thing) && thing->HasTC(0x1a)) {
            CTCInterface1A* c = thing->GetTC<CTCInterface1A>(0x1a);
            if (c->Armed)
                reinterpret_cast<CTCInventoryItem*>(c)->ReturnToInventory();
        }
        if (thing->HasTC(0x48)) {
            CTCWeapon* weapon = thing->GetTC<CTCWeapon>(0x48);
            if (weapon->IsProjectileWeapon())
                weapon->SetProjectileWeaponLoaded(false, 0);
        }
        if (thing->HasTC(0x83))
            thing->GetTC<CTCAnimatingObject>(0x83)->PlayDefaultAnimation();
        if (!IsPrimarySlotFree()) {
            CThing* next = GetThingInPrimarySlot();
            if (next && !(next->m_flags & 1) && next->HasTC(0x48) && next->GetTCConst<CTCWeapon>(0x48)->IsProjectile())
                RemoveThing(*next);
        }
        return thing;
    }
    return 0;
}