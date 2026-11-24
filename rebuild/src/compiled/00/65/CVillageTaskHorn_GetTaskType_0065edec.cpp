// Leaf: ?GetTaskType@CVillageTaskHorn@@UBE?AW4EVillageTaskType@@XZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKHORN
#include "engine/CVillageTaskHorn.h"
#pragma optimize("s",on)
EVillageTaskType CVillageTaskHorn::GetTaskType() const
{
    return (EVillageTaskType)4;
}
// Flatten route (flatten_pilot v4, NF324): CVillageTaskHorn_GetTaskType -> CVillageTaskHorn::GetTaskType, the RTTI-placed PDB member; class model: engine/CVillageTaskHorn.h under FABLE_RETAIL_CVILLAGETASKHORN (preamble of CCountedPointerCVillageTaskBase_CCountedPointerCVillageTaskHorn_0065db2a.cpp).
