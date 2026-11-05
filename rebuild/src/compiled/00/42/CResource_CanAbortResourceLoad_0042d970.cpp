// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?CanAbortResourceLoad@CResource_Methods@@QAE_NH@Z
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    bool CanAbortResourceLoad(int a0);
};
bool CResource_Methods::CanAbortResourceLoad(int a0) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?CanAbortResourceLoad@CResource@@UBE_NJ@Z); evidence: RTTI vtable slot + PDB name set.
