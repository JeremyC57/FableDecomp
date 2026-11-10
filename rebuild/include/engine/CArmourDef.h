#ifndef FABLE_ENGINE_CARMOURDEF_H
#define FABLE_ENGINE_CARMOURDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
class CArmourDef:public CParentDefClassBase {
public:unsigned char AugmentationResponse[12];std::map<EDamageAttribute,float> DamageTypeResponse;unsigned char ArmourThresholds[12];bool AllHitsKnockdown,AllHitsNegated,AllHitsCauseRecoil;long DamageMaterial,BlockMaterial;
};
FABLE_STATIC_ASSERT(offsetof(CArmourDef,DamageTypeResponse)==0x48);
FABLE_STATIC_ASSERT(sizeof(CArmourDef)==0x6c);
#endif
