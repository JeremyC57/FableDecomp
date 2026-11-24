// CInputTypeXboxPadRightStickEvent::IsInputTypeWithMouseButton (0x004456d0)
// Leaf: ?IsInputTypeWithMouseButton@CInputTypeXboxPadRightStickEvent@@UBE_NW4EMouseButtonControl@@@Z
#define FABLE_RETAIL_CINPUTTYPEXBOXPADRIGHTSTICKEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeXboxPadRightStickEvent.h"
bool CInputTypeXboxPadRightStickEvent::IsInputTypeWithMouseButton(enum EMouseButtonControl mouseButton) const
{
(void)mouseButton;
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CInputTypeXboxPadRightStickEvent_IsInputTypeWithMouseButton -> CInputTypeXboxPadRightStickEvent::IsInputTypeWithMouseButton, the RTTI-placed PDB member; class model: engine/CInputTypeXboxPadRightStickEvent.h under FABLE_RETAIL_CINPUTTYPEXBOXPADRIGHTSTICKEVENT_VIRTUAL_MODEL.
