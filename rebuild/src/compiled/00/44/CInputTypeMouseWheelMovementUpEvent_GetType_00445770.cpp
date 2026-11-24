// CInputTypeMouseWheelMovementUpEvent::GetType
// Leaf: ?GetType@CInputTypeMouseWheelMovementUpEvent@@UBE?AW4EInputType@NControlSystem@@XZ
// __fastcall virtual returning a constant type id (7); 'this' in ecx, unused.
#define FABLE_RETAIL_CINPUTTYPEMOUSEWHEELMOVEMENTUPEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeMouseWheelMovementUpEvent.h"
NControlSystem::EInputType CInputTypeMouseWheelMovementUpEvent::GetType() const
{
    return (NControlSystem::EInputType)7;
}
// Flatten route (flatten_pilot v4, NF324): CInputTypeMouseWheelMovementUpEvent_GetType -> CInputTypeMouseWheelMovementUpEvent::GetType, the RTTI-placed PDB member; class model: engine/CInputTypeMouseWheelMovementUpEvent.h under FABLE_RETAIL_CINPUTTYPEMOUSEWHEELMOVEMENTUPEVENT_VIRTUAL_MODEL.
