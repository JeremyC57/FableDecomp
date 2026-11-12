// CTCUnholyPower::FinalCleanup() -- end the spell's effects: kill (cut-scene cancel) or fade every hit effect and every per-creature effect, restore each live hit creature's lighting channel and lighting updates, clear its epic-spell flag and reset its hit state; then restore the hero's lighting channel and lighting updates
// Inline-budget: /d2inlT62 is the smallest T preserving native first-call/final-inline LowerBound decisions; T61 outlines both.
// Leaf: ?FinalCleanup@CTCUnholyPower@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00795090, 586 bytes, private thiscall (Ego_r.pdb ?FinalCleanup@CTCUnholyPower@@AAEXXZ; reached from OnKill 0x007952E0 / FinalCleanup jmp thunk 0x007952F0).
// Flags: /d2inlT62
// Callees: CTCSpecialAbilities::FadeOrKillEffect (__fastcall), CThing::Kill, IS_THING_ALIVE / CIntelligentPointer (0x00A01B10 /
//   0x00A01B50 / 0x00A01B90 / 0x00A01C10), CTCGraphicAppearance::SetLightingChannel, CThing::GetTC<CTCVolumeContainmentTracker>
//   (inline CVectorMap::LowerBound), CTCUnholyPower::ResetCreatureHitState 0x00793E00, the map iterator increment.
// Shape: written exactly like its landed near-twin 0x0078FAF0 CTCDivineWrath::FinalCleanup (same flag, same opt-ins; the
//   CTCUnholyPower opt-ins CANCELFORCUTSCENE / CTCSOUNDPLAYER / ADDANDCOPYTC only make the class header's inline StopSound compile).
//   The earlier local model swapped the HitEffects iterator (retail edi) with &it->Effect (retail esi).
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DIVINE_FINALCLEANUP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
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
#include "engine/CTCUnholyPower.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"
void CTCUnholyPower::FinalCleanup()
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