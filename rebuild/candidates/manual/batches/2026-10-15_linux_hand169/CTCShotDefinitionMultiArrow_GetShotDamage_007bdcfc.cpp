// CTCShotDefinitionMultiArrow::GetShotDamage() const -- the base shot damage scaled by the shooter's divine-wrath damage multiplier (interface 0x88) when the shooter is alive and has one
// Leaf: GetShotDamage
// Retail: Fable.exe 0x007BDCFC, 123 bytes (plain `ret`; size TU with frame: optimize("y", off) + ("s", on)).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (out-of-line
// `__fastcall`, by reference; the GetPItem result binds through a frame temporary), std::bitset::test @ 0x00410DE0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CTCDivineWrath::GetDamageMultiplier() @ 0x007CBF50,
// CTCShotDefinitionBase::GetShotDamage() @ 0x007BEE10 (manifest "CTCPhysicsBase::GetAirResistance" is propagated).
// Layout: retail CTCShotDefinitionBase (4 bytes below the PDB after HitThingsList): PShooter +0x28 (PDB 0x2C);
// CThing presence bitset +0x20, interface map +0x44.
// Shape: the multiplier is a float local initialised with fld1; the interface key temporary reuses the alive
// test's frame word; the lookup is a __forceinline template returning the component (landed 0x0064B668 shape).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                            // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                        // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                     // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (CThing*)
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                // +4
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CTCDivineWrath {
    float GetDamageMultiplier() const;              // 0x007CBF50
};
struct CTCShotDefinitionBase {
    void*                   __vftable;              // +0x00
    CThing*                 ParentThing;            // +0x04
    unsigned char           _pad08[0x28 - 0x08];
    CBaseIntelligentPointer PShooter;               // +0x28
    float GetShotDamage() const;                    // 0x007BEE10
};
struct CTCShotDefinitionMultiArrow : CTCShotDefinitionBase {
    float GetShotDamage() const;
};

float CTCShotDefinitionMultiArrow::GetShotDamage() const
{
    float multiplier = 1.0f;
    if (IS_THING_ALIVE<CThing*>(PShooter.GetPItem())) {
        CThing* shooter = PShooter.GetPItem();
        if (shooter->HasInterface(0x88))
            multiplier = shooter->GetInterfaceUnchecked<CTCDivineWrath>(0x88)->GetDamageMultiplier();
    }
    return CTCShotDefinitionBase::GetShotDamage() * multiplier;
}
