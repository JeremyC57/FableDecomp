// Leaf: ?GetSizeofClass@CChestDef_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 60. __fastcall this=ecx.
#include "engine/CChestDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CChestDef_Methods : CChestDef {
    int GetSizeofClass();
};
int CChestDef_Methods::GetSizeofClass() { return 60; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetSizeofClass@CChestDef@@UAEKXZ); evidence: RTTI vtable slot + PDB name set.
