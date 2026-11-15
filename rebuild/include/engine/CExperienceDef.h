#ifndef FABLE_ENGINE_CEXPERIENCEDEF_H
#define FABLE_ENGINE_CEXPERIENCEDEF_H
#ifdef FABLE_RETAIL_CEXPERIENCEDEF_PASS22
// Retail opt-in (Codex pass22): original FableWin TPI0x30747 and native orb members+0x28..0x44.
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_map.h"
class CExperienceDef : public CSubDefClassBase {
public:
 static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CExperienceDef",-1); }
 long OrbCombiningParticleEffect;
 std::map<long,float> ParticleScalesPerExperiencePoints;
 std::map<long,long> PickupParticleEffectsPerExperiencePoints;
 std::map<long,float> LifeTimeForDistanceThresholds;
};
FABLE_STATIC_ASSERT(offsetof(CExperienceDef,OrbCombiningParticleEffect)==0x28);
FABLE_STATIC_ASSERT(offsetof(CExperienceDef,LifeTimeForDistanceThresholds)==0x44);
FABLE_STATIC_ASSERT(sizeof(CExperienceDef)==0x50);
#endif
#endif
