// 006c6990 CTCHeroOpinionDeedLog::PostDeed
// Leaf: ?PostDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@ABVC3DVector@@PBVCThing@@PAVCThingAICreature@@W4EOpinionPostDeedType@@_N5PBVCFaction@@5J@Z
// Flags: none
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_POST_DEED_RANGE_OUTLINE
#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE
#define FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_DECL_PASS26
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_POST_DEED_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CTOR_PASS27
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_LIMBO
#define FABLE_RETAIL_CTCHERO
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CENEMYDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/ThingAlive.h"
#include "engine/COpinionDeed.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CEnemyDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorldMap.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
#include "engine/CTCHero.h"
#include "engine/CTCVillage.h"
#include "engine/CPlayerGui.h"
#include "engine/NGlobalConsole.h"

#include "engine/CWorld.h"
inline CCharString FABLE_FASTCALL CEnemyDef::StatGetTypeName() { return "CEnemyDef"; }
#ifdef _MSC_VER
// The 18-byte CThingAICreature pointer constructor at 004d67e3 owns the
// 28-byte deleting destructor at 004d6931. Both use the size profile.
#pragma optimize("s",on)
#endif
template<> inline CIntelligentPointer<CThingAICreature>::CIntelligentPointer() {}
template CIntelligentPointer<CThingAICreature>::CIntelligentPointer();
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
COpinionDeed::COpinionDeed(EOpinionDeedType type,long handle,const C3DVector& position,const CThing* target,bool happening,bool crime,long data)
 : DeedType(type),DeedHandle(handle),Position(position),FrameHappened(CWorld::GetFrame()),PTarget(target),WasSearchingForWitnesses(happening),IsStillHappening(happening),IsCrimeReportedToVillage(false),IsCrime(crime),ExtraData(data) {}
CWitnessesSearchingDeed::CWitnessesSearchingDeed(long handle,EOpinionDeedType type,CThingAICreature* recipient,EOpinionPostDeedType post,const CFaction* faction,bool reverse)
 : DeedHandle(handle),DeedType(type),Recipient(recipient),PostType(post),PFaction(faction),ReverseFactionSearch(reverse) {}

long CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType deed_type,const C3DVector& position,const CThing* target,CThingAICreature* recipient,EOpinionPostDeedType post_type,bool is_crime,bool keep_searching_for_witnesses,const CFaction* pfaction,bool reverse_faction_search,long extra_data)
{
 if(deed_type==OPINION_DEED_TYPE_NONE || post_type==OPINION_POST_DEED_TYPE_NULL)return -1;
 bool allow_tutorial=true;
 const CRegion& region=GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(GetWorldMap().GetMapNumberAt(GetParentThing().GetPos())));
 CDefPointer<const CRegionDef> pregion_def=region.GetPRegionDef();
 if(pregion_def.Get()){
  if(deed_type==OPINION_DEED_TYPE_CRIME_WEAPON_OUT){
   if(!pregion_def.Get()->WeaponOutCrime || !pregion_def.Get()->UseCrimes)return -1;
  }
  if(!pregion_def.Get()->UseCrimes){
   allow_tutorial=false;is_crime=false;
   if(deed_type==OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)extra_data=-1;
  }
  if(!pregion_def.Get()->DisplayCrimes)allow_tutorial=false;
 }else{
  if(deed_type==OPINION_DEED_TYPE_CRIME_WEAPON_OUT)return -1;
  allow_tutorial=false;is_crime=false;
  if(deed_type==OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)extra_data=-1;
 }
 if(deed_type==OPINION_DEED_TYPE_CRIME_ASSAULT || deed_type==OPINION_DEED_TYPE_CRIME_GBH || deed_type==OPINION_DEED_TYPE_CRIME_MURDER){
  if(target && !target->HasTC(TCI_CREATURE_OPINION_OF_HERO)){allow_tutorial=false;is_crime=false;}
  CDefPointer<const CEnemyDef> enemy_def;
  if(target && target->GetPBaseDef()->GetSubDef(&enemy_def) && enemy_def->Faction!=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FACTION_VILLAGERS"))allow_tutorial=false;
 }
 if(GetGuardsIgnoreCrimes()==true){is_crime=false;allow_tutorial=false;if(deed_type==OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)extra_data=-1;}
 CTCVillage* tc_village;
 if(IS_THING_ALIVE(VillageForCrimes) && VillageForCrimes->GetTC(&tc_village) && tc_village->GetVillageLimbo())allow_tutorial=false;
 long new_handle=NextHandle++;
 COpinionDeed* new_deed=new COpinionDeed(deed_type,new_handle,position,target,keep_searching_for_witnesses,is_crime,extra_data);
 OpinionDeedMap.insert(std::make_pair(new_handle,new_deed));
 if(keep_searching_for_witnesses)
  OpinionDeedsSearchingForWitnesses.push_back(CWitnessesSearchingDeed(new_handle,deed_type,recipient,post_type,pfaction,reverse_faction_search));
 if(!InformRelevantThingsOfDeed(new_handle,recipient,post_type,pfaction,reverse_faction_search)&&!keep_searching_for_witnesses){
  delete new_deed;OpinionDeedMap.erase(new_handle);--NextHandle;new_handle=-1;
 }
 if(allow_tutorial && NGlobalConsole::GEnableTutorials && !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()){
  CTCHero* tc_hero=0;
  if(PeekPDeedEffectsDef(deed_type)->Tutorial && GetParentThing().GetTC(&tc_hero))
   tc_hero->GiveTutorialForCategory(PeekPDeedEffectsDef(deed_type)->Tutorial,false);
 }
 return new_handle;
}