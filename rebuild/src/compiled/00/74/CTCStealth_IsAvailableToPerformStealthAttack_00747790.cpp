// CTCStealth::IsAvailableToPerformStealthAttack(CThing* target) -- true when the owner carries a live weapon in its primary slot and that weapon can stealth-attack the target
// Leaf: IsAvailableToPerformStealthAttack
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747790, length 141 bytes (`ret 4`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (two inlined GetTC lookups: 0x46 CTCCarrying = bit 6 of
// CThing+0x28, 0x48 CTCWeapon = bit 8), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsCapableOfStealthAttackBy(CThing*)
// @ 0x005DAD60.
// Layout: CThing interface bitset +0x20, interface map +0x44, dead flag +0x91 bit 0.
// Shape: both components come through the out-parameter GetInterface template: the two lookups then share one key frame word and the
// first bit test stays `test byte ptr [eax+28h],40h` (HasTC + GetTC gave two key slots and `mov cl,[eax+28h]`); the weapon's result is
// returned directly.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
class CTCWeapon { public:
    bool IsCapableOfStealthAttackBy(CThing* thing) const;   // 0x005DAD60
};
class CTCStealth { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool IsAvailableToPerformStealthAttack(CThing* target);
};

bool CTCStealth::IsAvailableToPerformStealthAttack(CThing* target)
{
    const CTCCarrying* carrying;
    if (ParentThing->PeekTC(TCI_CARRYING, &carrying)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon && !weapon->IsDead()) {
            const CTCWeapon* tc;
            if (weapon->PeekTC(TCI_WEAPON, &tc))
                return tc->IsCapableOfStealthAttackBy(target);
        }
    }
    return false;
}