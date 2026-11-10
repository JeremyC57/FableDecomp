#ifndef FABLE_ENGINE_CCOMBOMULTIPLIERDATA_H
#define FABLE_ENGINE_CCOMBOMULTIPLIERDATA_H
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
// Layout: Ego_r fields0/16/20, native0/12/16 after map shrinks to12 bytes.
class CComboMultiplierData {
public:
    std::map<EDamageAttribute,float> ComboMultiplier;
    float ExperienceMultiplierNumSecondsForAttenuation;
    float ExperienceMultiplierParamDecayDeltaPerSecond;
};
#endif
