// CTCFollowed::IsCreatureValidFollower -- check whether a creature can follow the owner.
// Retail: 0x006D4E60, 363 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?IsCreatureValidFollower@CTCFollowed@@QBE_NABVCThingAICreature@@PA_N@Z
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_IS_ALLOWED_TO_FOLLOW
#define FABLE_RETAIL_CTHING_SCRIPTSUPERVISION
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_FOLLOWER_LIMIT
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#include "engine/CThingAICreature.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCCreatureOpinionOfHero.h"
bool CTCFollowed::IsCreatureValidFollower(const CThingAICreature& creature,bool* pis_ai_follower)const
{
 bool local_bool;
 bool& is_ai_follower=pis_ai_follower==0?local_bool:*pis_ai_follower;
 is_ai_follower=!creature.IsUnderScriptSupervision() && GetParentThing().HasTC(TCI_HERO_OPINION_DEED_LOG) && creature.HasTC(TCI_CREATURE_OPINION_OF_HERO);
 if(creature.IsAllowedToFollowHero()){
  if(is_ai_follower){
   const CTCHeroStats* ptc_hero_stats;
   GetParentThing().PeekTC(&ptc_hero_stats);
   if(!(IsFollower(&creature)||GetNumFollowers()<ptc_hero_stats->GetMaxFollowersForRenownLevel(ptc_hero_stats->GetRenownLevel())))return false;
   const CTCCreatureOpinionOfHero* ptc_opinion;
   creature.PeekTC(&ptc_opinion);
   const CTCFollowed* ptc_followed;
   GetParentThing().PeekTC(&ptc_followed);
   return ptc_followed->IsFollower(&creature)?ptc_opinion->IsWillingToContinueFollowingHero():ptc_opinion->IsWillingToStartFollowingHero();
  }
  return true;
 }
 return false;
}