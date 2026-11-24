// CInputTypeXboxPadRightStickEvent::GetType
// Leaf: ?GetType@CInputTypeXboxPadRightStickEvent@@UBE?AW4EInputType@NControlSystem@@XZ
// Retail 0x004456b0: mov eax, 4 ; ret
// __fastcall: this in ecx (unused). Returns constant type id 4.
#define FABLE_RETAIL_CINPUTTYPEXBOXPADRIGHTSTICKEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeXboxPadRightStickEvent.h"
NControlSystem::EInputType CInputTypeXboxPadRightStickEvent::GetType() const
{
    return (NControlSystem::EInputType)4;
}
// Flatten route (flatten_pilot v4, NF324): CInputTypeXboxPadRightStickEvent_GetType -> CInputTypeXboxPadRightStickEvent::GetType, the RTTI-placed PDB member; class model: engine/CInputTypeXboxPadRightStickEvent.h under FABLE_RETAIL_CINPUTTYPEXBOXPADRIGHTSTICKEVENT_VIRTUAL_MODEL.
