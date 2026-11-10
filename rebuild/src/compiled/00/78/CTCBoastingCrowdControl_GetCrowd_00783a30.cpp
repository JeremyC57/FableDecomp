// CTCBoastingCrowdControl::GetCrowd -- retrieve or create the hero's boasting crowd.
// Retail: 0x00783A30, 393 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?GetCrowd@CTCBoastingCrowdControl@@AAE_NAAVCThing@@PAPAV2@PAPAVCTCCreatureGroupBoastingCrowd@@_N@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCBoastingCrowdControl.h"
#include "engine/CTCCreatureGroupBoastingCrowd.h"
#include "engine/CTCDInternalMarker.h"
#include "engine/ThingAlive.h"
bool CTCBoastingCrowdControl::GetCrowd(CThing& hero_creature,CThing** ppcrowd_thing,CTCCreatureGroupBoastingCrowd** pptc_crowd,bool create_as_needed)
{
 std::map<unsigned __int64,CIntelligentPointer<CThing> >::iterator crowd_it=HeroCrowdThings.find(hero_creature.GetUniqueID());
 if(crowd_it!=HeroCrowdThings.end()) {
  *ppcrowd_thing=crowd_it->second.Get();
  if(IS_THING_ALIVE(*ppcrowd_thing)) {
   (*ppcrowd_thing)->GetTC(pptc_crowd);
   return true;
  }
  HeroCrowdThings.erase(crowd_it);
 }
 if(create_as_needed) {
  long def_num=PeekWorld().PeekDefinitionManager().GetDefGlobalIndexFromName("INTERNAL_MARKER_LIGHT");
  *ppcrowd_thing=CTCDInternalMarker::Create(def_num,GetParentThing().GetPos());
  *pptc_crowd=GFDebugPDynamicCast((*ppcrowd_thing)->AddTC("CTCCreatureGroupBoastingCrowd",false,0),(CTCCreatureGroupBoastingCrowd*)0);
  (*pptc_crowd)->SetHero(hero_creature);
  HeroCrowdThings.insert(std::make_pair(hero_creature.GetUniqueID(),*ppcrowd_thing));
  return true;
 }
 return false;
}