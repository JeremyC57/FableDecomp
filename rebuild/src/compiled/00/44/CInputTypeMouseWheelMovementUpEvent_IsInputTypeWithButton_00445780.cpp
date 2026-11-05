// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsInputTypeWithButton@CInputTypeMouseWheelMovementUpEvent_Methods@@QAE_NH@Z
#include "engine/CInputTypeMouseWheelMovementUpEvent.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputTypeMouseWheelMovementUpEvent_Methods : CInputTypeMouseWheelMovementUpEvent {
    bool IsInputTypeWithButton(int a0);
};
bool CInputTypeMouseWheelMovementUpEvent_Methods::IsInputTypeWithButton(int a0) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInputTypeWithButton@CInputTypeMouseWheelMovementUpEvent@@UBE_NW4EXboxControllerButton@@@Z); evidence: RTTI vtable slot + PDB name set.
