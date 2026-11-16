// 009482a0 CAIStateGroup_GoToPub::GetSeat
// Leaf: ?GetSeat@CAIStateGroup_GoToPub@@IAE_NPAVCThing@@PBVCCharString@@@Z
// Flags: none
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCTAVERNTABLE_SEAT
#define FABLE_RETAIL_CTCTAVERNTABLE_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCTAVERNTABLE_FULL_LAYOUT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CAIBrain.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCTavernTable.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/GFRandom.h"

bool CAIStateGroup_GoToPub::GetSeat(CThing* table,const CCharString* text)
{
 if(table&&text){
  PTable=CIntelligentPointer<CThingObject>(GFDebugPDynamicCast(table,(CThingObject*)0));
  CTCTavernTable* tc_tavern_table;
  if(PTable->GetTC(&tc_tavern_table)&&tc_tavern_table->GetSeat(GetCreature(),Pos,RHSet,ActionPointIndex,SeatIndex,text)){
   // Fidelity: named ordinary pointer with the original temporary lifetime.
   { CIntelligentPointer<CThing> tankard(tc_tavern_table->GetTankard(SeatIndex));PTankard=tankard; }
   return true;
  }
  ActionPointIndex=-1;
  return false;
 }
 std::list<CThingObject*> tables;
 CTCThingOwner* tc_thing_owner;
 if(IS_THING_ALIVE(PPub)&&PPub->GetTC(&tc_thing_owner)){
  const std::list<CIntelligentPointer<CThing> >& objects=tc_thing_owner->GetObjectList();
  for(std::list<CIntelligentPointer<CThing> >::const_iterator object_it=objects.begin();object_it!=objects.end();++object_it){
   if(IS_THING_ALIVE(*object_it)&&(*object_it)->HasTC(TCI_TAVERN_TABLE))tables.push_back(GFDebugPDynamicCast((*object_it).Get(),(CThingObject*)0));
  }
 }else return false;
 if(tables.empty())return false;
 PTable=CIntelligentPointer<CThingObject>((CThingObject*)0);
 while(!tables.empty()){
  long index=GFRandom(tables.size(),GetPWorldSeed());
  std::list<CThingObject*>::iterator table_it;
  for(table_it=tables.begin();table_it!=tables.end();++table_it){
   if(index==0){PTable=CIntelligentPointer<CThingObject>(*table_it);break;}
   --index;
  }
  CTCTavernTable* tc_tavern_table;
  if(PTable->GetTC(&tc_tavern_table)&&tc_tavern_table->GetSeat(GetCreature(),Pos,RHSet,ActionPointIndex,SeatIndex,0)){
   PTankard=CIntelligentPointer<CThing>(tc_tavern_table->GetTankard(SeatIndex));
   return true;
  }
  tables.erase(table_it);
 }
 ActionPointIndex=-1;
 return false;
}