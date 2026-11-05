// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsInputTypeWithButton@CInputTypeXboxPadLeftStickEvent_Methods@@QAE_NH@Z
#include "engine/CInputTypeXboxPadLeftStickEvent.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputTypeXboxPadLeftStickEvent_Methods : CInputTypeXboxPadLeftStickEvent {
    bool IsInputTypeWithButton(int a0);
};
bool CInputTypeXboxPadLeftStickEvent_Methods::IsInputTypeWithButton(int a0) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInputTypeWithButton@CInputTypeXboxPadLeftStickEvent@@UBE_NW4EXboxControllerButton@@@Z); evidence: RTTI vtable slot + PDB name set.
