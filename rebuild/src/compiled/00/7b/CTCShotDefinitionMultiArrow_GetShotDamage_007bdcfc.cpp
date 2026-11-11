// CTCShotDefinitionMultiArrow::GetShotDamage() const -- the base shot damage scaled by the shooter's divine-wrath damage multiplier (interface 0x88) when the shooter is alive and has one
// Leaf: GetShotDamage
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad1[0x23] removed; map pointer made const (the row grades the const LowerBound).
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe 0x007BDCFC, 123 bytes (plain `ret`; size TU with frame: optimize("y", off) + ("s", on)).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (out-of-line
// `__fastcall`, by reference; the GetPItem result binds through a frame temporary), std::bitset::test @ 0x00410DE0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CTCDivineWrath::GetDamageMultiplier() @ 0x007CBF50,
// CTCShotDefinitionBase::GetShotDamage() @ 0x007BEE10 (manifest "CTCPhysicsBase::GetAirResistance" is propagated).
// Layout: retail CTCShotDefinitionBase (4 bytes below the PDB after HitThingsList): PShooter +0x28 (PDB 0x2C);
// CThing presence bitset +0x20, interface map +0x44.
// Shape: the multiplier is a float local initialised with fld1; the interface key temporary reuses the alive
// test's frame word; the lookup is a __forceinline template returning the component (landed 0x0064B668 shape).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> m_present;                        // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> FABLE_FORCEINLINE T* GetInterfaceUnchecked(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        if (it == map->end() || type < it->first)
            it = map->end();
        return (T*)it->second;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (CThing*)
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
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
    if (IS_THING_ALIVE<CThing*>(((CThing*)PShooter.GetPItem()))) {
        CThing* shooter = ((CThing*)PShooter.GetPItem());
        if (shooter->HasInterface(0x88))
            multiplier = shooter->GetInterfaceUnchecked<CTCDivineWrath>(0x88)->GetDamageMultiplier();
    }
    return CTCShotDefinitionBase::GetShotDamage() * multiplier;
}