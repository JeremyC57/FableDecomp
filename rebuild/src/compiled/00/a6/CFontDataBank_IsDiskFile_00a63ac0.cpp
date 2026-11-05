// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsDiskFile@CFontDataBank_Methods@@QAE_NHH@Z
#include "engine/CFontDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CFontDataBank_Methods : CFontDataBank {
    bool IsDiskFile(int a0, int a1);
};
bool CFontDataBank_Methods::IsDiskFile(int a0, int a1) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsDiskFile@CFontDataBank@@MAE_NKK@Z); evidence: RTTI vtable slot + PDB name set.
