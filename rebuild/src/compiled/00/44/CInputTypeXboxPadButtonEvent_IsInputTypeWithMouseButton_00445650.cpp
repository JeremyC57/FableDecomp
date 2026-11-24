// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsInputTypeWithMouseButton@CInputTypeXboxPadButtonEvent@@UBE_NW4EMouseButtonControl@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CInputTypeXboxPadButtonEvent.h under FABLE_RETAIL_CINPUTTYPEXBOXPADBUTTONEVENT_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CInputTypeXboxPadButtonEvent_Methods::IsInputTypeWithMouseButton); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CINPUTTYPEXBOXPADBUTTONEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeXboxPadButtonEvent.h"  // retyped onto the PDB layout; byte parity re-verified
bool CInputTypeXboxPadButtonEvent::IsInputTypeWithMouseButton(EMouseButtonControl a0) const { return false; }
