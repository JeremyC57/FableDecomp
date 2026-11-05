// Constant-true predicate: `mov al,1; ret`. __fastcall this=ecx, no args.
// Leaf: ?IsParentDef@CParentDefClassBase_Methods@@QAE_NXZ
#include "engine/CParentDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CParentDefClassBase_Methods : CParentDefClassBase {
    bool IsParentDef();
};
bool CParentDefClassBase_Methods::IsParentDef() { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsParentDef@CParentDefClassBase@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
