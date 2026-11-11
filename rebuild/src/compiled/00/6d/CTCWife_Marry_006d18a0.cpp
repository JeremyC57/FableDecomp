// CTCWife::Marry(bool) -- move into the marital home, update relationships and reputation, then start the wedding fade
// Leaf: ?Marry@CTCWife@@QAEX_N@Z
// Retail: 006d18a0,1330 code bytes; package extent1344 includes14 padding bytes; thiscall ret4.
// Flags: /GS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MARRIAGE
#define FABLE_RETAIL_CTCHEROSTATS_ADDRENOWN
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCWife.h"
#include "engine/CHeroMarriageDef.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CGameEvent.h"
#include "engine/CRGBColour.h"
#include "engine/CPlayerGui.h"
#include "engine/ThingAlive.h"
#include "engine/CTCWife_Marry_bodies.h"
void CTCWife::Marry(bool instant_with_no_cutscene)
{
 CThingAICreature& creature=static_cast<CThingAICreature&>(GetParentThing());
 CThingBuilding* marital_home=FindMaritalHome();
 if(marital_home) {
  CTCVillageMember* ptc_village_member;
  GetParentThing().GetTC(&ptc_village_member);
  if(!ptc_village_member->GetPVillage()) {
   CTCVillageMember* ptc_home_village_member=0;
   if(marital_home->GetTC(&ptc_home_village_member)) {
    CTCVillage* ptc_village=ptc_home_village_member->GetPVillage();
    if(ptc_village) {
     ptc_village_member->SetPVillage(0);
     ptc_village->AddThingToVillage(GetParentThing());
    }
   }
  }
  creature.SetHomeBuilding(marital_home);
  CTCBuyableHouse* ptc_buyable_house=0;
  marital_home->GetTC(&ptc_buyable_house);
  ptc_buyable_house->SetWifeLivingHere(&creature);
 }
 long cur_frame=CWorld::GetFrame();
 FrameLastAwareOfHusband=cur_frame;
 FrameGotMarriedToThePlayer=cur_frame;
 FrameLastReducedOpinion=cur_frame;
 FrameLastEvaluatedGiftOpinion=cur_frame+5;
 FrameLastConsideredGivingGift=cur_frame+13;
 CThingPlayerCreature* pplayer=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 HouseDressingLevelLastCommentedOn=-1;
 if(IS_THING_ALIVE(pplayer)) {
  CheckAppearanceChanges(*pplayer);
  ClearAllAppearanceChangedNoticed();
 }
 if(IS_THING_ALIVE(pplayer)) {
  CTCHeroStats* ptc_hero_stats;
  pplayer->GetTC(&ptc_hero_stats);
  ptc_hero_stats->AddNewMarriage(static_cast<CThingAICreature*>(&GetParentThing()),marital_home,(EMarriageState)1);
  ptc_hero_stats->AddRenown((long)PHeroMarriageDef->HeroStatRenownIncreasePerMarriage);
 }
 CTCVillageMember* ptc_village_member=0;
 if(GetParentThing().GetTC(&ptc_village_member)) {
  CTCVillage* ptc_village=0;
  ptc_village=ptc_village_member->GetPVillage();
  if(ptc_village) {
   CTCCreatureOpinionOfHero* ptc_village_opinion=0;
   if(ptc_village->GetParentThing().GetTC(&ptc_village_opinion))
    ptc_village_opinion->AdjustOpinionTransient((EOpinion)1,0.5f,0,CMainGameComponent::GetConstantFPS()*300,0.2f,false);
   ptc_village->DetachFromDummyVillager(&GetParentThing());
  }
 }
 CTCShopKeeper* tc_shopkeeper;
 if(GetParentThing().GetTC(&tc_shopkeeper))tc_shopkeeper->DetachFromShop();
 CTCCreatureOpinionOfHero* ptc_opinion=0;
 if(GetParentThing().GetTC(&ptc_opinion)) {
  ptc_opinion->SetOpinionReactionMask("OPINION_REACTION_MASK_WIFE");
  ptc_opinion->SetOpinionDeedMask("OPINION_DEED_MASK_WIFE");
  ptc_opinion->SetPersonalityOverride("OPINION_PERSONALITY_WIFE_NORMAL",true);
  ptc_opinion->SetForcedAttitude((EOpinionAttitudeType)18);
  ptc_opinion->ClearAllKnownDeeds();
  ptc_opinion->HeroOpinionEnemy=false;
  OpinionAttitude=GetWorld().GetOpinionReactionManager().GetAttitudeType(GetParentThing(),0);
  AttitudeUpdateCounter=0;
 }
 creature.SetWorkBuilding(0);
 if(instant_with_no_cutscene) {
  InitWifeNonpersistentState();
  InitWifeAtEndOfMarriageFrescoStartFadeIn();
  InitWifeAtEndOfMarriageFrescoEndFadeIn();
 } else {
  AddToGlobalTCs();
  CMainGameComponent::Get()->GetDisplayEngine().StartScreenFadeOut(PHeroMarriageDef->WeddingTimeForScreenToFadeOut,PHeroMarriageDef->WeddingTimeForScreenToFadeIn,CRGBColour(0,0,0),-1.0f);
  SendPauseEvent(true);
  WaitingForFadeOut=true;
 }
 CPlayerGui::Get().UpdateMiniMapMarkers(false);
}