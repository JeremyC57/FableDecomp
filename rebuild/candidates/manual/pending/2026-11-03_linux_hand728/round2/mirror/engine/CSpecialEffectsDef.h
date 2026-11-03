#ifndef FABLE_ENGINE_CSPECIALEFFECTSDEF_H
#define FABLE_ENGINE_CSPECIALEFFECTSDEF_H
#include "engine/CSubDefClassBase.h"
class CCharString;
#include "stlport/fable_map.h"
class CSpecialEffectsStringMap {
public: std::map<unsigned long,long> ContainmentMap;
};
// Original TPI0x17a69; original SpecialEffectsMap28 retained at native sizec.
class CSpecialEffectsDef : public CSubDefClassBase {
public:
    unsigned char _pad_0x25[3];
    CSpecialEffectsStringMap SpecialEffectsMap;
    bool GetEffectFromString(const CCharString&,long&) const;
};
FABLE_STATIC_ASSERT(sizeof(CSpecialEffectsDef)==0x34);
#endif
