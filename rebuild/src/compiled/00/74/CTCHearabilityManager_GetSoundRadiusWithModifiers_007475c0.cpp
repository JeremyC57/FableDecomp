// CTCHearabilityManager::GetSoundRadiusWithModifiers() const -- the hero-experience sound-radius multiplier (interface 0x68, else 1) times the base sound radius of the thing's interface-4 component (+0xA8), or the multiplier alone when that component is absent
// Leaf: GetSoundRadiusWithModifiers
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007475C0, length 150 bytes (+10 int3 padding), returns float on the x87 stack.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined interface lookups),
// CTCHeroExperience::GetSoundRadiusMultiplier() const @ 0x006D65E0.
// Shape: two inlined GetInterface lookups through one templated `__forceinline` accessor (bit 8 of CThing +0x2C = interface
// 0x68, `test ch,1`; bit 4 of +0x20 = interface 4); the multiplier is a float local preset to 1.0f whose frame word the first
// lookup key reuses; `fld mult ; fmul [c+0A8h]` is `c->radius * mult`.
// Layout: CTCBase ParentThing +0x04; CThing component bitset +0x20, interface map +0x44. The interface-4 component's float
// at +0xA8 is its base sound radius (descriptive).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCHeroExperience {
public:
    float GetSoundRadiusMultiplier() const;     // 0x006D65E0
};
class CTCSoundSource {                          // interface 4
public:
    unsigned char _pad00[0xa8];
    float BaseSoundRadius;                      // +0xa8
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
};
class CTCHearabilityManager : public CTCBase {
public:
    float GetSoundRadiusWithModifiers() const;
};

float CTCHearabilityManager::GetSoundRadiusWithModifiers() const
{
    float multiplier = 1.0f;
    const CTCHeroExperience* experience;
    if (ParentThing->PeekTC(TCI_HERO_EXPERIENCE, &experience))
        multiplier = experience->GetSoundRadiusMultiplier();
    const CTCSoundSource* source;
    if (ParentThing->PeekTC(TCI_HERO_STATS, &source))
        return source->BaseSoundRadius * multiplier;
    return multiplier;
}