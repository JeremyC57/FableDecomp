// CInputTypeXboxPadButtonEvent::GetType
// Leaf: ?GetType@CInputTypeXboxPadButtonEvent@@UBE?AW4EInputType@NControlSystem@@XZ
// __fastcall (this in ecx), no stack args, returns constant enum value 2.
#define FABLE_RETAIL_CINPUTTYPEXBOXPADBUTTONEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeXboxPadButtonEvent.h"
NControlSystem::EInputType CInputTypeXboxPadButtonEvent::GetType() const
{
    return (NControlSystem::EInputType)2;
}
// Flatten route (flatten_pilot v4, NF324): CInputTypeXboxPadButtonEvent_GetType -> CInputTypeXboxPadButtonEvent::GetType, the RTTI-placed PDB member; class model: engine/CInputTypeXboxPadButtonEvent.h under FABLE_RETAIL_CINPUTTYPEXBOXPADBUTTONEVENT_VIRTUAL_MODEL.
