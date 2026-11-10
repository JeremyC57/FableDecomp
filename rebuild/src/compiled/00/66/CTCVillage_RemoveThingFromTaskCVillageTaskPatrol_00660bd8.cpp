// CTCVillage::RemoveThingFromTask<CVillageTaskPatrol> -- remove a participating thing and erase the task when requested.
// Leaf: ??$RemoveThingFromTask@VCVillageTaskPatrol@@@CTCVillage@@ABEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@AAV?$list@V?$CCountedPointer@VCVillageTaskPatrol@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskPatrol@@@@@std@@@std@@@Z
// Retail: 0x00660bd8, 123 bytes, thiscall.
// Flags: none
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_REMOVE_TASK
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_LIST
#include "engine/CTCVillage.h"
#include "engine/CIsTask.h"
#define FABLE_RETAIL_CVILLAGETASKPATROL
#include "engine/CVillageTaskPatrol.h"
#include "stlport/fable_algorithm.h"
template<class T>
void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, std::list<CCountedPointer<T> >& task_list) const
{
    if (task->IsThingInvolvedWithTask(thing)) {
        bool remove_task;
        task->RemoveThingFromTask(thing,remove_task);
        if (remove_task) {
            typename std::list<CCountedPointer<T> >::iterator task_it = std::find_if(task_list.begin(),task_list.end(),CIsTask(*task.Get()));
            task_list.erase(task_it);
        }
    }
}
template void CTCVillage::RemoveThingFromTask<CVillageTaskPatrol>(CIntelligentPointer<CVillageTaskBase>,const CThing&,std::list<CCountedPointer<CVillageTaskPatrol> >&) const;