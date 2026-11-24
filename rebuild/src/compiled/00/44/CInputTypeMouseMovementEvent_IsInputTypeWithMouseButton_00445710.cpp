// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsInputTypeWithMouseButton@CInputTypeMouseMovementEvent@@UBE_NW4EMouseButtonControl@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CInputTypeMouseMovementEvent.h under FABLE_RETAIL_CINPUTTYPEMOUSEMOVEMENTEVENT_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CInputTypeMouseMovementEvent_Methods::IsInputTypeWithMouseButton); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CINPUTTYPEMOUSEMOVEMENTEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeMouseMovementEvent.h"  // retyped onto the PDB layout; byte parity re-verified
bool CInputTypeMouseMovementEvent::IsInputTypeWithMouseButton(EMouseButtonControl a0) const { return false; }
