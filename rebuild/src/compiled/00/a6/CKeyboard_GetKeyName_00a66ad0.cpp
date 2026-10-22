// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CKeyboard.h"  // retyped onto the PDB layout; byte parity re-verified
struct CKeyboard_Methods : CKeyboard {
    bool Get(int a0, int text);
};
bool CKeyboard_Methods::Get(int a0, int text) { return false; }