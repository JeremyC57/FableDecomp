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
class CTCBase;
struct CPresenceBits {
    bool test(long bit);                            // 0x00410DE0 (std::bitset::test)
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                        // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                     // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        if (it == map->last || type < it->first)
            it = map->last;
        return (T*)it->second;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE_0040dce0(const T& thing);   // 0x0040DCE0 (CThing*)
class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                // +4
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
class CTCDivineWrath { public:
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
    if (IS_THING_ALIVE_0040dce0<CThing*>(((CThing*)PShooter.GetPItem()))) {
        CThing* shooter = ((CThing*)PShooter.GetPItem());
        if (shooter->HasInterface(0x88))
            multiplier = shooter->GetInterfaceUnchecked<CTCDivineWrath>(0x88)->GetDamageMultiplier();
    }
    return CTCShotDefinitionBase::GetShotDamage() * multiplier;
}