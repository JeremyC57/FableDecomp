#pragma optimize("s",on)
// CTCVillage::EnableVillagerDefTypes -- enable or disable each matching villager dummy.
// Inline-budget: plain source calls Find; /d2inlS100 changes other native calls. Existing scoped Find body opt-in retains native LowerBound calls.
// Leaf: ?EnableVillagerDefTypes@CTCVillage@@QAEX_NABVCCharString@@@Z
// Retail: 0065b142,386 bytes,thiscall.
// Flags: /Os /Oy-
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_ENABLE_DEF_TYPES
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCVillage.h"
#include "engine/CTCDummyVillager.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CDefIndex.h"
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 if(thing!=0) { if(!thing->IsDead())return true; }
 return false;
}
void CTCVillage::EnableVillagerDefTypes(bool enable,const CCharString& def_name) {
 CDefIndex def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName(def_name);
 for(std::list<CIntelligentPointer<CThing> >::iterator i=VillageDummies.begin();i!=VillageDummies.end();++i) {
  CThing* dummy=i->Get();
  if(IS_THING_ALIVE(dummy) && dummy->HasTC(TCI_DUMMY_VILLAGER)) {
   CTCDummyVillager* dummy_villager=static_cast<CTCDummyVillager*>(&dummy->GetTC(TCI_DUMMY_VILLAGER));
   if(dummy_villager->GetDefType()==def_index) {
    dummy_villager->SetRespawnable(enable);
    if(!enable) {
     if(IS_THING_ALIVE(dummy_villager->GetCreature()))dummy_villager->GetCreature()->Kill(false);
    } else if(!IS_THING_ALIVE(dummy_villager->GetCreature())) {
     C3DVector creation_pos;
     if(dummy->HasTC(TCI_NAVIGATION)) {
      CTCCreatureNavigation* tc_creature_navigation=static_cast<CTCCreatureNavigation*>(&dummy->GetTC(TCI_NAVIGATION));
      tc_creature_navigation->GetNearbyWanderPosition(GetParentThing().GetPos(),50.0f,creation_pos,true);
     } else creation_pos=ParentThing->GetPos();
     dummy_villager->CreateVillager(*this,creation_pos,false);
    }
   }
  }
 }
}