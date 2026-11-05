// CTCSpecialAbilities::GetCurrentAbilityStrafeAnim() const -- the strafe animation name of the spell component behind the current ability (fireball 0x10, will lightning 0xb), else ""
// Leaf: GetCurrentAbilityStrafeAnim
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B870, 194 bytes (`ret 4`: the CCharString result's hidden pointer).
// Callees: the const interface LowerBound @ 0x004365B0 (inlined lookups of interfaces 0x9d and 0x94), the fireball spell's
// strafe-animation getter @ 0x007C85F0 (CTCFireballSpell area) and the will-lightning one @ 0x0079F610 (CTCWillLightning area;
// both catalogued under the propagated label CTCFireballSpell::GetReleaseAnimName -- named descriptively), CCharString(const
// char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E).
// Layout: CTCSpecialAbilities CurrentAbility +0x10; CThing interface bitset +0x20 (0x9d = bit 29 and 0x94 = bit 20 of the fifth
// word), map +0x44.
// Shape: a two-case switch (tests in value order, bodies in source order: 0x10 first); each case is the out-parameter
// GetInterface (component in eax, then `mov ecx,eax`) and returns the getter's CCharString straight through the hidden
// pointer; the default builds the empty string there.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCFireballSpell { public:
    CCharString GetStrafeAnimName() const;             // 0x007C85F0
};
class CTCWillLightning { public:
    CCharString GetStrafeAnimName() const;             // 0x0079F610
};
class CTCSpecialAbilities { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    CCharString GetCurrentAbilityStrafeAnim() const;
};

CCharString CTCSpecialAbilities::GetCurrentAbilityStrafeAnim() const
{
    switch (CurrentAbility) {
    case 0x10: {
        const CTCFireballSpell* fireball;
        if (ParentThing->PeekTC(TCI_FIREBALL_SPELL, &fireball))
            return fireball->GetStrafeAnimName();
        break;
    }
    case 0xb: {
        const CTCWillLightning* lightning;
        if (ParentThing->PeekTC(TCI_WILL_LIGHTNING, &lightning))
            return lightning->GetStrafeAnimName();
        break;
    }
    }
    return CCharString("", -1);
}