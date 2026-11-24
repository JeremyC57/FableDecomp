// CInputTypeXboxPadLeftStickEvent::GetType @ 0x00445670
// Leaf: ?GetType@CInputTypeXboxPadLeftStickEvent@@UBE?AW4EInputType@NControlSystem@@XZ
// Virtual event-type accessor: returns the constant type id 3.
// __fastcall (this in ecx) but 'this' is unused; no stack args => ret 0.
#define FABLE_RETAIL_CINPUTTYPEXBOXPADLEFTSTICKEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeXboxPadLeftStickEvent.h"
NControlSystem::EInputType CInputTypeXboxPadLeftStickEvent::GetType() const
{
    return (NControlSystem::EInputType)3;
}
// Flatten route (flatten_pilot v4, NF324): CInputTypeXboxPadLeftStickEvent_GetType -> CInputTypeXboxPadLeftStickEvent::GetType, the RTTI-placed PDB member; class model: engine/CInputTypeXboxPadLeftStickEvent.h under FABLE_RETAIL_CINPUTTYPEXBOXPADLEFTSTICKEVENT_VIRTUAL_MODEL.
