#ifndef FABLE_ENGINE_CSPECIALABILITIESDRAINLIFEDATADEF_H
#define FABLE_ENGINE_CSPECIALABILITIESDRAINLIFEDATADEF_H
#ifdef FABLE_RETAIL_DRAIN_PULSE_PASS29
#include "engine/CSubDefClassBase.h"
#include "fable_string.h"
class CSpecialAbilitiesDrainLifeDataDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return "CSpecialAbilitiesDrainLifeDataDef"; }
    float Damage;
};
FABLE_STATIC_ASSERT(sizeof(CSpecialAbilitiesDrainLifeDataDef)==44);
#endif
#endif
