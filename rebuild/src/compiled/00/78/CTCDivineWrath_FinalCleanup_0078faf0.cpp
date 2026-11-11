// CTCDivineWrath::FinalCleanup -- kill or fade spell effects, restore creature lighting and clear epic-spell state
// Inline-budget: /d2inlT62 is the smallest T preserving native first-call/final-inline LowerBound decisions; T61 outlines both.
// Leaf: ?FinalCleanup@CTCDivineWrath@@AAEXXZ
// Retail: 0078faf0,586 bytes; private thiscall.
// Flags: /d2inlT62
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DIVINE_FINALCLEANUP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDivineWrath.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"
void CTCDivineWrath::FinalCleanup()
{
 {
  for(std::vector<PerEffectInfo>::iterator it=HitEffects.begin(),endit=HitEffects.end();it!=endit;++it) {
   PerEffectInfo& info=*it;
   if(CutSceneCancel) {
    if(IS_THING_ALIVE(info.Effect))info.Effect->Kill(true);
   } else CTCSpecialAbilities::FadeOrKillEffect(info.Effect);
  }
 }
 std::map<CThingCreatureBase*,PerCreatureInfo>::iterator it,endit;
 for(it=LastFrameCreaturesHit.begin(),endit=LastFrameCreaturesHit.end();it!=endit;++it) {
  if(CutSceneCancel) {
   if(IS_THING_ALIVE(it->second.Effect))it->second.Effect->Kill(true);
  } else CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
  CIntelligentPointer<CThingCreatureBase> creature=it->second.ActualCreature;
  if(IS_THING_ALIVE(creature)) {
   NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=creature->GetTCGraphicAppearance();
   if(ptc_graphic_appearance)ptc_graphic_appearance->SetLightingChannel(it->second.DefaultChannel,true,true);
   CTCVolumeContainmentTracker* ptc_volume_contain=0;
   if(creature->GetTC(&ptc_volume_contain))ptc_volume_contain->SetUpdateLightingChannels(true);
   creature->SetUnderInfluenceOfEpicSpell(false);
  }
  ResetCreatureHitState(creature);
 }
 NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=GetParentThing().GetTCGraphicAppearance();
 if(ptc_graphic_appearance)ptc_graphic_appearance->SetLightingChannel(HeroDefaultChannel,true,true);
 CTCVolumeContainmentTracker* ptc_volume_contain=0;
 if(GetParentThing().GetTC(&ptc_volume_contain))ptc_volume_contain->SetUpdateLightingChannels(true);
}