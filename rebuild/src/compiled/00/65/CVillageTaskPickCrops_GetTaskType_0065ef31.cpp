// Leaf: ?GetTaskType@CVillageTaskPickCrops@@UBE?AW4EVillageTaskType@@XZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS
#include "engine/CVillageTaskPickCrops.h"
#pragma optimize("s",on)
EVillageTaskType CVillageTaskPickCrops::GetTaskType() const
{
    return (EVillageTaskType)2;
}
// Flatten route (flatten_pilot v4, NF324): CVillageTaskPickCrops_GetTaskType -> CVillageTaskPickCrops::GetTaskType, the RTTI-placed PDB member; class model: engine/CVillageTaskPickCrops.h under FABLE_RETAIL_CVILLAGETASKPICKCROPS (preamble of CCountedPointerCVillageTaskBase_CCountedPointerCVillageTaskPickCrops_0065db5c.cpp).
