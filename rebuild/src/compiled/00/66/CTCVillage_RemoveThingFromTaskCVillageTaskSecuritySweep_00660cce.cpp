// CTCVillage::RemoveThingFromTask<CVillageTaskSecuritySweep> -- remove a participating thing and erase the task when requested.
// Leaf: ??$RemoveThingFromTask@VCVillageTaskSecuritySweep@@@CTCVillage@@ABEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@AAV?$list@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@@std@@@std@@@Z
// Retail: 0x00660cce, 123 bytes, thiscall.
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
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP
#include "engine/CVillageTaskSecuritySweep.h"
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
template void CTCVillage::RemoveThingFromTask<CVillageTaskSecuritySweep>(CIntelligentPointer<CVillageTaskBase>,const CThing&,std::list<CCountedPointer<CVillageTaskSecuritySweep> >&) const;