#ifndef FABLE_ENGINE_CHEROEXPERIENCEDEF_H
#define FABLE_ENGINE_CHEROEXPERIENCEDEF_H
// Retail opt-in (Codex pass 17): raw Ego_r class has eleven STLport containers, each four bytes smaller in retail.
#include "engine/CSubDefClassBase.h"
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#include "engine/CHeroStatIncreaseData.h"
#include "engine/EDamageAttribute.h"
#endif
class CHeroExperienceDef:public CSubDefClassBase {public:
 unsigned char ExperienceLevelThresholds[12],HeroLevelTitles[12],FallbackMultipliers[12];
 unsigned char SoundIntensityThresholds[12],ComboMultiplierMap[12];
 float ExperienceMultiplierParamPerUnitComboMultiplier,ExperienceMultiplierRegionChangeMultiplier;
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
 // Retail opt-in (Codex pass21): original map type proven by both PDBs and native OnHit accesses.
 unsigned char ComboMultiplierIncrease[12],ComboMultiplierLock[12],ComboMultiplierLost[12],ComboMultiplierRunningHitsIncrease[12];
 std::map<EDamageAttribute,CHeroStatIncreaseData> StatIncreasePerHitType;
 unsigned char AttackHistoryCombos[12];
#else
 unsigned char ComboMultiplierIncrease[12],ComboMultiplierLock[12],ComboMultiplierLost[12],ComboMultiplierRunningHitsIncrease[12],StatIncreasePerHitType[12],AttackHistoryCombos[12];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CHeroExperienceDef)==0xb4);
#endif
