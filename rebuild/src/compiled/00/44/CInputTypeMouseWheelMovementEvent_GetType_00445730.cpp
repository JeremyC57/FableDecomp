// CInputTypeMouseWheelMovementEvent::GetType
// Leaf: ?GetType@CInputTypeMouseWheelMovementEvent@@UBE?AW4EInputType@NControlSystem@@XZ
// Retail 0x00445730: mov eax,6 ; ret
// __fastcall (this in ecx) returning a constant type id.
#define FABLE_RETAIL_CINPUTTYPEMOUSEWHEELMOVEMENTEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeMouseWheelMovementEvent.h"
NControlSystem::EInputType CInputTypeMouseWheelMovementEvent::GetType() const
{
    return (NControlSystem::EInputType)6;
}
// Flatten route (flatten_pilot v4, NF324): CInputTypeMouseWheelMovementEvent_GetType -> CInputTypeMouseWheelMovementEvent::GetType, the RTTI-placed PDB member; class model: engine/CInputTypeMouseWheelMovementEvent.h under FABLE_RETAIL_CINPUTTYPEMOUSEWHEELMOVEMENTEVENT_VIRTUAL_MODEL.
