// Keep working while a task is available and the worker can continue.
// Leaf: ?Update@CAIStateGroup_WorkInWarehouse@@UAEXXZ
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_ASSERTED_BODY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_FILTER
#define FABLE_RETAIL_CAISTATEGROUP_MOVEGOODSBASE
#define FABLE_RETAIL_CVILLAGETASKBASE
#define FABLE_RETAIL_CVILLAGETASKCARRYCRATE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CRATETASK
#define FABLE_RETAIL_CTCVILLAGE_WAREHOUSE_UPDATE_PASS27
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "engine/EMovementType.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CAIStateGroup_WorkInWarehouse.h"
#include "engine/CAIBrain.h"

void CAIStateGroup_WorkInWarehouse::Update()
{
    while (PTask.Get() && !IsStopping()) {
        const CTCVillageMember* ptc_village_mem;
        GetCreature().PeekTC(&ptc_village_mem);
        CTCVillage* village = ptc_village_mem->GetPVillage();
        if (village) {
            if (village->SetCrateTaskAsAcceptedByThing(*PTask, GetCreature())) {
                if (MoveCrates()) {
                    if (PTask.Get())
                        village->SetCrateTaskAsCompleted(PTask);
                }
            }
            PTask = village->GetBestCrateTaskForThing(GetCreature());
        }
    }
    Terminate();
}