// CTCInventoryWeapons::GetActiveMeleeWeaponDamage() const -- the active melee weapon's damage while that weapon thing is alive, else 0
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed; map pointer made const (the row grades the const LowerBound).
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C4769, length 108 bytes. Frame-pointer size TU: optimize("s", on)
// (`fldz ; fstp` zero). Ego_r.pdb: ?GetActiveMeleeWeaponDamage@CTCInventoryWeapons@@QBEMXZ.
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound @ 0x004365B0 (the inline CThing::GetInterface for interface 0x48,
// the weapon component), the weapon's damage getter @ 0x005DAAA0 (manifest "CTCWeapon::GetAnimationSpeedValue"; name descriptive).
// Shape: the zero result, the GetInterface key and the accessor's unset component (read back on the miss path) all share the
// one frame word [ebp-4]; the hit path assigns the result, which stays in st(0) for the single `return damage`.
// Layout: PActiveMeleeWeapon at retail +0x134 (as the landed RefreshActiveWeaponData 0x005C30D3).
// Naming (NF191, 2026-11-20, q4492): the row-local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (members CIntelligentPointer<CThing>, the Ego_r.pdb member type) and the IS_THING_ALIVE template
//   declaration (the out-of-line instance 0x004CC340, as the landed CTCInventoryWeapons rows declare it): the liveness tests bind the
//   FableWin.pdb ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z. Other row-local
//   models unchanged (in place).
#include "fable_compiler.h"
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || wanted < it->first)
                it = map->end();
            *out = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // 0x004CC340
class CTCWeapon { public:                            // interface type 0x48
    float GetDamage() const;                  // 0x005DAAA0
};

class CTCInventoryWeapons { public:
    unsigned char _pad00[0x134];
    CIntelligentPointer<CThing> PActiveMeleeWeapon;   // +0x134
    float GetActiveMeleeWeaponDamage() const;
};

float CTCInventoryWeapons::GetActiveMeleeWeaponDamage() const
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(PActiveMeleeWeapon))
        damage = ((CThing*)((CBaseIntelligentPointer*)&PActiveMeleeWeapon)->GetPItem())->GetTC<CTCWeapon>(0x48)->GetDamage();
    return damage;
}