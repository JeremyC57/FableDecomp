// Leaf: ?GetTaskType@CVillageTaskCarryCrate@@UBE?AW4EVillageTaskType@@XZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKCARRYCRATE
#include "engine/CVillageTaskCarryCrate.h"
#pragma optimize("s",on)
EVillageTaskType CVillageTaskCarryCrate::GetTaskType() const
{
    return (EVillageTaskType)5;
}
// Flatten route (flatten_pilot v4, NF324): CVillageTaskCarryCrate_GetTaskType -> CVillageTaskCarryCrate::GetTaskType, the RTTI-placed PDB member; class model: engine/CVillageTaskCarryCrate.h under FABLE_RETAIL_CVILLAGETASKCARRYCRATE (preamble of CCountedPointerCVillageTaskBase_CCountedPointerCVillageTaskCarryCrate_0065db8e.cpp).
