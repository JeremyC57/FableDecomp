// CTCVillage::UpdateHornTasks -- add missing horn tasks at curfew, remove them afterward.
// Inline-budget: /d2inlS10 expands the original implicit predicate copy; S20 also expands its owning constructor.
// Retail: 0065be6c,336 bytes.
// Leaf: ?UpdateHornTasks@CTCVillage@@AAEXXZ
// Flags: /d2inlS10
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_HORN_TASKS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CVILLAGETASKHORN
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#include "engine/CThing.h"
#include "engine/CVillageDef.h"
#include "engine/CTCVillage.h"
#include "engine/CGameTimeManager.h"
#include "engine/CWorld.h"
#include "engine/CIsHornTaskWithThisHorn.h"
#include "stlport/fable_algorithm.h"
#pragma optimize("y",off)
#pragma optimize("s",on)
void CTCVillage::UpdateHornTasks()
{
 if(PeekWorld().PeekGameTimeManager().IsTimeBetween(GetPDef()->CurfewStartTime,GetPDef()->CurfewFinishTime)) {
  if(!HornTasksAdded) {
   for(std::list<CIntelligentPointer<CThing> >::iterator horn_it=VillageHorns.begin();horn_it!=VillageHorns.end();++horn_it) {
    CIntelligentPointer<CThing>&p_horn=*horn_it;
    std::list<CCountedPointer<CVillageTaskHorn> >::const_iterator task_it=std::find_if(HornTasks.begin(),HornTasks.end(),CIsHornTaskWithThisHorn(p_horn));
    if(task_it==HornTasks.end())HornTasks.push_back(CCountedPointer<CVillageTaskHorn>(new CVillageTaskHorn(*p_horn)));
   }
   HornTasksAdded=true;
  }
 } else if(HornTasksAdded) {
  for(std::list<CIntelligentPointer<CThing> >::iterator horn_it=VillageHorns.begin();horn_it!=VillageHorns.end();++horn_it)
   RemoveThingFromAllTasks(*horn_it->Get());
  HornTasksAdded=false;
 }
}