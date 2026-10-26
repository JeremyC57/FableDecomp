// CTCCarrying::RemoveThingFromPrimarySlot(bool) -- take the thing out of the primary carry slot (weapons are dropped, other things removed), notify its components, and remove a following melee weapon left in the slot; returns the removed thing
// Leaf: RemoveThingFromPrimarySlot
// Retail 0x006A1E00, 384 bytes, `ret 4` (the argument is unused).
// Callees: CTCCarrying's primary-slot empty test 0x006A1270 (labelled CThing_Array_GetFirstPhysicalSwitch),
// GetThingInPrimarySlot 0x006A1440, DropWeapon(CThing*, CTCCarrying*, CThing*) 0x00845CE0 (`__fastcall`),
// CTCCarrying::RemoveThing(CThing*) 0x006A19B0, CTCCarrying::IsCarryingThing(CThing*) 0x006A1060, CVectorMap::LowerBound
// 0x0040F020 (interfaces 0x1A, 0x48 weapon, 0x83 animating object) and const 0x004365B0, the 0x1A component's hook
// 0x005D8E50, CTCWeapon::IsProjectileWeapon 0x005DAA40 / SetProjectileWeaponLoaded(bool, CThing*) 0x005DAC50 /
// IsMeleeWeapon 0x005DAA70, CTCAnimatingObject::PlayDefaultAnimation 0x007F8F30.
// Layout: CTCBase ParentThing +4; CThing bitset +0x20, map +0x44, dead flag +0x91; the 0x1A component's armed pointer +0x10.
// Shape: the whole body sits in `if (!IsPrimarySlotFree()) { ... return thing; } return 0;` (an early `return 0` is laid
// out first); HasTC + ternary GetTC lookups sharing one key slot; `this` is homed in the frame for the final RemoveThing.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    SEntry* LowerBound(const long* type) const;         // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTCConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCInterface1A {
    unsigned char _pad00[0x10];
    void*         Armed;                                // +0x10
    void OnUseFailed();                                 // 0x005D8E50
};
class CTCWeapon { public:
    void SetProjectileWeaponLoaded(bool loaded, CThing* ammo);   // 0x005DAC50
    bool IsMeleeWeapon();                               // 0x005DAA70
public:
    bool IsProjectileWeapon() const;   // 0x005DAA40
};
class CTCAnimatingObject { public:
    void PlayDefaultAnimation();                        // 0x007F8F30
};
class CTCCarrying;
void __fastcall DropWeapon(CThing* weapon, CTCCarrying* carrying, CThing* owner);   // 0x00845CE0
class CTCCarrying { public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    void RemoveThing(CThing* thing);                    // 0x006A19B0
    bool IsCarryingThing(CThing* thing);                // 0x006A1060
    CThing* RemoveThingFromPrimarySlot(bool unused);
public:
    bool IsPrimarySlotFree() const;   // 0x006A1270
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

CThing* CTCCarrying::RemoveThingFromPrimarySlot(bool unused)
{
    if (!IsPrimarySlotFree()) {
        CThing* thing = GetThingInPrimarySlot();
        if (thing->HasTC(0x48))
            DropWeapon(thing, this, ParentThing);
        else
            RemoveThing(thing);
        if (!IsCarryingThing(thing) && thing->HasTC(0x1a)) {
            CTCInterface1A* c = thing->GetTC<CTCInterface1A>(0x1a);
            if (c->Armed)
                c->OnUseFailed();
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
            if (next && !(next->m_flags & 1) && next->HasTC(0x48) && next->GetTCConst<CTCWeapon>(0x48)->IsMeleeWeapon())
                RemoveThing(next);
        }
        return thing;
    }
    return 0;
}