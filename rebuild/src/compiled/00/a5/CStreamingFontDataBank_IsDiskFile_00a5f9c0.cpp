// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?IsDiskFile@CStreamingFontDataBank_Methods@@QAE_NHH@Z
#include "engine/CStreamingFontDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CStreamingFontDataBank_Methods : CStreamingFontDataBank {
    bool IsDiskFile(int a0, int a1);
};
bool CStreamingFontDataBank_Methods::IsDiskFile(int a0, int a1) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsDiskFile@CStreamingFontDataBank@@MAE_NKK@Z); evidence: RTTI vtable slot + PDB name set.
