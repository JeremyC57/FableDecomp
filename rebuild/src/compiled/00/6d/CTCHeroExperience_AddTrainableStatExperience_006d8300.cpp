// CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory,long) -- collect scaled trainable experience and forward it to hero stats
// Leaf: ?AddTrainableStatExperience@CTCHeroExperience@@QAEXW4EHeroExperienceStatCategory@@J@Z
// Retail: 0x006D8300,173 bytes,thiscall
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_PICKUP_MULTIPLIER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_ADDEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/CThing.h"
#include "engine/CHeroExperienceDef.h"
#include "engine/CAnimSet.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHeroStats.h"
#include "engine/CAnimSet.h"
namespace NHeroExperience {float FABLE_FASTCALL GetWeaponModifiedExperienceMultiplierForThing(const CThing&);}
void CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory stat_type,long experience)
{
 if(AbleToGainExperience){
  long experience_to_collect=GetExperiencePickUpMultiplier()*experience;
  experience_to_collect=(long)(NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(GetParentThing())*experience_to_collect);
  StatExperiencePoints[stat_type]+=experience_to_collect;
  GFLimitLower(StatExperiencePoints[stat_type],0);
  CTCHeroStats* HeroStats;
  if(GetParentThing().GetTC(&HeroStats))HeroStats->AddHeroStatExperience(stat_type,experience_to_collect);
 }
}