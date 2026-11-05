// CTCSpecialAbilities::ToggleUsingAbility(long ability) -- for ability 4, cancel the thing's active physical-shield spell move; true when one was cancelled
// Leaf: ToggleUsingAbility
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072AC90, length 93 bytes (`ret 4`; caller 0x005EEAE8).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the shield component's active test
// @ 0x0079DCB0 (manifest "C3DMeshInfo::HasPhysicsMesh", propagated from the byte-identical getter) and
// CTCPhysicalShieldSpell::CancelMove @ 0x0079E300.
// Layout: the parent thing's interface bitset at +0x20 (type 0x93 -> word 4 = +0x30, bit 19 = 0x80000) and its sorted
// interface map at +0x44 (the landed GetInterface shape of 0x007504E0).
// Shape: the inline out-parameter GetInterface keeps the map and then the component in esi; nested ifs share the
// `xor al,al` exit. GetInterface must be `__forceinline`: as a plain inline template VC7.1 emits it out of line here.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPhysicalShieldSpell { public:
    bool IsMoveActive();                                    // 0x0079DCB0
    void CancelMove();                                      // 0x0079E300
};

class CTCSpecialAbilities { public:
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool ToggleUsingAbility(long ability);
};

bool CTCSpecialAbilities::ToggleUsingAbility(long ability)
{
    if (ability == 4) {
        CTCPhysicalShieldSpell* shield;
        if (ParentThing->GetTC(TCI_PHYSICAL_SHIELD_SPELL, &shield)) {
            if (shield->IsMoveActive()) {
                shield->CancelMove();
                return true;
            }
        }
    }
    return false;
}