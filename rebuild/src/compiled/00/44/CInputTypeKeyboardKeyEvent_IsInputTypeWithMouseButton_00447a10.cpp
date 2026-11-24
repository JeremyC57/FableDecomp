// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsInputTypeWithMouseButton@CInputTypeKeyboardKeyEvent@@UBE_NW4EMouseButtonControl@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CInputTypeKeyboardKeyEvent.h under FABLE_RETAIL_CINPUTTYPEKEYBOARDKEYEVENT_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CInputTypeKeyboardKeyEvent_Methods::IsInputTypeWithMouseButton); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CINPUTTYPEKEYBOARDKEYEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeKeyboardKeyEvent.h"  // retyped onto the PDB layout; byte parity re-verified
bool CInputTypeKeyboardKeyEvent::IsInputTypeWithMouseButton(EMouseButtonControl a0) const { return false; }
