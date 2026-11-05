// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsInputTypeWithMouseButton@CInputTypeKeyboardKeyEvent_Methods@@QAE_NH@Z
#include "engine/CInputTypeKeyboardKeyEvent.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputTypeKeyboardKeyEvent_Methods : CInputTypeKeyboardKeyEvent {
    bool IsInputTypeWithMouseButton(int a0);
};
bool CInputTypeKeyboardKeyEvent_Methods::IsInputTypeWithMouseButton(int a0) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInputTypeWithMouseButton@CInputTypeKeyboardKeyEvent@@UBE_NW4EMouseButtonControl@@@Z); evidence: RTTI vtable slot + PDB name set.
