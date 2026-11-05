// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?GetKeyName@CKeyboard_Methods@@QAE_NHH@Z
#include "engine/CKeyboard.h"  // retyped onto the PDB layout; byte parity re-verified
struct CKeyboard_Methods : CKeyboard {
    bool GetKeyName(int a0, int text);
};
bool CKeyboard_Methods::GetKeyName(int a0, int text) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetKeyName@CKeyboard@@UBE_NW4EInputKey@@AAVCWideString@@@Z); evidence: RTTI vtable slot + PDB name set.
