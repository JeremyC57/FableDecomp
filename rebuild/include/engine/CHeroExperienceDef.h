#ifndef FABLE_ENGINE_CHEROEXPERIENCEDEF_H
#define FABLE_ENGINE_CHEROEXPERIENCEDEF_H
// Retail opt-in (Codex pass 17): raw Ego_r class has eleven STLport containers, each four bytes smaller in retail.
#include "engine/CSubDefClassBase.h"
class CHeroExperienceDef:public CSubDefClassBase {public:
 unsigned char ExperienceLevelThresholds[12],HeroLevelTitles[12],FallbackMultipliers[12];
 unsigned char SoundIntensityThresholds[12],ComboMultiplierMap[12];
 float ExperienceMultiplierParamPerUnitComboMultiplier,ExperienceMultiplierRegionChangeMultiplier;
 unsigned char ComboMultiplierIncrease[12],ComboMultiplierLock[12],ComboMultiplierLost[12],ComboMultiplierRunningHitsIncrease[12],StatIncreasePerHitType[12],AttackHistoryCombos[12];
};
FABLE_STATIC_ASSERT(sizeof(CHeroExperienceDef)==0xb4);
#endif
