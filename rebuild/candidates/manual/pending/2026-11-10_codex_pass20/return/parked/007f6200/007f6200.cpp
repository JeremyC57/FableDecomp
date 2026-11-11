// 007f6200 CTCTargetingAI::UpdatePBestEnemyCreatureForAttack
// Grade: DIFFER(1458v1458) -- parent/owner spill14/18 inverted; comparator store/call scheduling, filter temporaries and later parent caching;36 strict hunks
// Retail: 007f6200,1458 bytes.
// Inline-budget: native007f6686 calls this conversion; plain expands it; T40..60 also changes unrelated helpers.
// Flags: none
// Leaf: ?UpdatePBestEnemyCreatureForAttack@CTCTargetingAI@@QAEX_N@Z
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_MANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CTargetingDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CTCTurncoat.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCEnemy.h"
#include "engine/NCreatureMode.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/ThingAlive.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CTCTargetingAI_Filters.h"
#include "engine/CThingCompare_BestCreatureToAttack.h"
#include "engine/CFilter_Bind2.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
void CTCTargetingAI::UpdatePBestEnemyCreatureForAttack(bool force_update)
{
 const C3DVector& pos=GetParentThing().GetPos();
 const CThingAICreature& parent_creature=*GFDebugPDynamicCast(&GetParentThing(),(const CThingAICreature*)0);
 bool overridden_by_script=false;
 CThingCreatureBase* pold_best_enemy=PBestEnemyCreatureForAttack.Get();
 if(IS_THING_ALIVE(PScriptTargetOverride)&&!NCreatureMode::IsIncapacitated(*PScriptTargetOverride)) {
  const CTCTurncoat* ptc_turncoat;
  if(GetParentThing().PeekTC(&ptc_turncoat)&&ptc_turncoat->IsTurncoated()) {
   if(CTCEnemy::AreThingsEnemies(parent_creature,*PScriptTargetOverride)) {
    PBestEnemyCreatureForAttack=PScriptTargetOverride;
    overridden_by_script=true;
   }
  } else {
   PBestEnemyCreatureForAttack=PScriptTargetOverride;
   overridden_by_script=true;
  }
 }
 if(!overridden_by_script) {
  std::vector<const CThing*> exclusion_list;
  if(!ThingsToNotAttack.empty()) {
   exclusion_list.reserve(ThingsToNotAttack.size());
   for(std::list<CIntelligentPointer<CThing> >::iterator it=ThingsToNotAttack.begin();it!=ThingsToNotAttack.end();++it) {
    CThing* pthing=it->Get();
    exclusion_list.push_back(pthing);
   }
  }
  const CTCPerceiveThing* tc_perception;
  parent_creature.PeekTC(&tc_perception);
  float radius=tc_perception->GetExtendedSightRadius();
  if(PeekWorldMap().IsPosInMap(pos)) {
   float dis_to_player=(CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetPos()-pos).GetMagnitude2D();
   long update_frequency_attenuator=1+4*(long)(dis_to_player/radius);
   CThingCreatureBase* pbest_for_attack=PBestEnemyCreatureForAttack.Get();
   if(force_update||((unsigned long)GetWorld().GetFrame()+GetParentThing().GetUniqueID())%(unsigned long)(CMainGameComponent::GetConstantFPS()*update_frequency_attenuator)==0) {
    pbest_for_attack=0;
    float prefer_player_weight=GetTargetingDef().PreferPlayerWeight;
    float prefer_non_combatant_weight=GetTargetingDef().PreferNonCombatantsWeight;
    float pref_last_attacker=GetTargetingDef().PreferLastAttackerWeight;
    float pref_cur_target=GetTargetingDef().PreferCurrentBestEnemyWeight;
    CThingCompare_BestCreatureToAttack comparison_filter(parent_creature,PBestEnemyCreatureForAttack.Get(),prefer_player_weight,prefer_non_combatant_weight,pref_last_attacker,pref_cur_target);
    CThingPlayerCreature* pplayer_creature=GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    CThingAICreature* pthing_ai_creature=0;
    {
     CThingFilter_In2DRange search_range(pos,radius);
     CThingFilter_IsValidAttackTargetForCreature search_attack(parent_creature,exclusion_list,true);
     if(!PeekThingSearchTools().GetPBestLeafThing(GFFilter_Bind2(search_range,search_attack),comparison_filter,&pthing_ai_creature))pthing_ai_creature=0;
    }
    if(IS_THING_ALIVE(pplayer_creature)) {
     CThingFilter_In2DRange range_filter(pos,radius);
     CThingFilter_IsValidAttackTargetForCreature attack_filter(parent_creature,exclusion_list,true);
     if(!range_filter(*pplayer_creature)||!attack_filter(*pplayer_creature))pplayer_creature=0;
    } else pplayer_creature=0;
    if(pplayer_creature&&pthing_ai_creature)pbest_for_attack=comparison_filter.operator()<CThingCreatureBase>(pthing_ai_creature,pplayer_creature);
    else if(pplayer_creature)pbest_for_attack=pplayer_creature;
    else if(pthing_ai_creature)pbest_for_attack=pthing_ai_creature;
   }
   if(PBestEnemyCreatureForAttack!=pbest_for_attack)PBestEnemyCreatureForAttack=pbest_for_attack;
   if(!IS_THING_ALIVE(PBestEnemyCreatureForAttack)&&IS_THING_ALIVE(parent_creature.GetPLastAttackedByCreature())&&CThingFilter_IsValidAttackTargetForCreature(parent_creature,exclusion_list,false)(*parent_creature.GetPLastAttackedByCreature())) {
    CThingCreatureBase* attacker=parent_creature.GetPLastAttackedByCreature();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    CIntelligentPointer<CThingCreatureBase> target(attacker);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    PBestEnemyCreatureForAttack=target;
   }
   if(!IS_THING_ALIVE(PBestEnemyCreatureForAttack)&&tc_perception->IsAwareOfThing()) {
    const CThing* pperceived_thing=tc_perception->GetPerceivedThing();
    if(IS_THING_ALIVE(pperceived_thing)&&(pperceived_thing->GetTypeFlags()&8)) {
     const CThingCreatureBase* pCreatureConst=GFDebugPDynamicCast(pperceived_thing,(const CThingCreatureBase*)0);
     CThingCreatureBase* pCreature=const_cast<CThingCreatureBase*>(pCreatureConst);
     if(CThingFilter_IsValidAttackTargetForCreature(parent_creature,exclusion_list,false)(*pCreature))PBestEnemyCreatureForAttack=pCreature;
    }
   }
  }
 }
 if(PBestEnemyCreatureForAttack!=pold_best_enemy) {
  if(IS_THING_ALIVE(pold_best_enemy)&&(pold_best_enemy->GetTypeFlags()&32))CPlayerGui::Get().UpdateMiniMapMarkers(false);
  else if(IS_THING_ALIVE(PBestEnemyCreatureForAttack)&&(PBestEnemyCreatureForAttack->GetTypeFlags()&32))CPlayerGui::Get().UpdateMiniMapMarkers(false);
 }
}
