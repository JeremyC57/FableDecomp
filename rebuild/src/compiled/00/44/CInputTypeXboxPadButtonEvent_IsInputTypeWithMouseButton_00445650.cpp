// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsInputTypeWithMouseButton@CInputTypeXboxPadButtonEvent_Methods@@QAE_NH@Z
#include "engine/CInputTypeXboxPadButtonEvent.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputTypeXboxPadButtonEvent_Methods : CInputTypeXboxPadButtonEvent {
    bool IsInputTypeWithMouseButton(int a0);
};
bool CInputTypeXboxPadButtonEvent_Methods::IsInputTypeWithMouseButton(int a0) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInputTypeWithMouseButton@CInputTypeXboxPadButtonEvent@@UBE_NW4EMouseButtonControl@@@Z); evidence: RTTI vtable slot + PDB name set.
