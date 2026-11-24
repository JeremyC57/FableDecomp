// Refresh the village torch tasks after a day/night change.
// Leaf: ?UpdateTorchTasks@CTCVillage@@AAEXXZ
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_UPDATE_TORCH_PASS27
#define FABLE_RETAIL_CVILLAGETASKBASE
#define FABLE_RETAIL_CVILLAGETASKTORCH
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_ISDAY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCLIGHT_ACTIVE_PASS27
#define FABLE_RETAIL_CLIGHTDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_DECL_PASS26
#define FABLE_RETAIL_STLP_BITSET_TEST_OWNER_PASS27
#include "fable_assert.h"
#include "engine/CTCVillage.h"
#include "engine/CThing.h"
#include "engine/CRGBColour.h"
#include "engine/CTCLight.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CVillageTaskTorch.h"

#pragma optimize("y", off)
#pragma optimize("s", on)

void CTCVillage::UpdateTorchTasks()
{
    static long next_frame_update = CMainGameComponent::GetConstantFPS() * 5;
    if (TorchTasksUpdateFrame < CWorld::GetFrame()) {
        TorchTasksUpdateFrame = CWorld::GetFrame() + next_frame_update;
        bool light_active_state_to_change = !PeekWorld().PeekGameTimeManager().IsDay();
        if (light_active_state_to_change != LightActiveStateToChange) {
            LightActiveStateToChange = light_active_state_to_change;
            MaxFrameToAssignTask = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 60;
        }
        for (std::list<CIntelligentPointer<CThing> >::iterator it = VillageTorches.begin(); it != VillageTorches.end(); ++it) {
            CThing* ptorch = it->Get();
            CTCLight* ptc_light;
            FABLE_RETAIL_ASSERT(ptorch != 0);
            if (!ptorch->IsDead() && ptorch->GetTC(&ptc_light)) {
                if (ptc_light->IsActive() != light_active_state_to_change && !IsThingInTaskList(*ptorch, TorchTasks)) {
                    if (VillagePeople.size() == 0)
                        ptc_light->SetActive(!ptc_light->IsActive());
                    else
                        TorchTasks.push_back(CCountedPointer<CVillageTaskTorch>(new CVillageTaskTorch(*ptorch)));
                } else if (ptc_light->IsActive() == light_active_state_to_change && IsThingInTaskList(*ptorch, TorchTasks)) {
                    RemoveThingFromTaskList(*ptorch, TorchTasks);
                } else if (ptc_light->IsActive() != light_active_state_to_change &&
                           IsThingInTaskList(*ptorch, TorchTasks) && !IsTaskTaken(*ptorch, TorchTasks)) {
                    FABLE_RETAIL_ASSERT(MaxFrameToAssignTask != -1);
                    if (CWorld::GetFrame() > MaxFrameToAssignTask) {
                        ptc_light->SetActive(!ptc_light->IsActive());
                        RemoveThingFromTaskList(*ptorch, TorchTasks);
                    }
                }
            }
        }
    }
}