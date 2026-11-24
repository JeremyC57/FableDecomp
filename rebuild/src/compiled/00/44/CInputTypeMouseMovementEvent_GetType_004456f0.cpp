// Leaf: ?GetType@CInputTypeMouseMovementEvent@@UBE?AW4EInputType@NControlSystem@@XZ
#define FABLE_RETAIL_CINPUTTYPEMOUSEMOVEMENTEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeMouseMovementEvent.h"
NControlSystem::EInputType CInputTypeMouseMovementEvent::GetType() const
{
    return (NControlSystem::EInputType)5;
}
// Flatten route (flatten_pilot v4, NF324): CInputTypeMouseMovementEvent_GetType -> CInputTypeMouseMovementEvent::GetType, the RTTI-placed PDB member; class model: engine/CInputTypeMouseMovementEvent.h under FABLE_RETAIL_CINPUTTYPEMOUSEMOVEMENTEVENT_VIRTUAL_MODEL.
