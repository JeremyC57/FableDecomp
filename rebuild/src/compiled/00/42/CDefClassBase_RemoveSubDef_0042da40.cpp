// Empty one-argument member: `ret 4`. __fastcall this=ecx, one stack arg.
// Leaf: ?RemoveSubDef@CDefClassBase_Methods@@QAEXPBD@Z
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase_Methods : CDefClassBase {
    void RemoveSubDef(const char* name);
};
void CDefClassBase_Methods::RemoveSubDef(const char* name) {}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?RemoveSubDef@CDefClassBase@@UAEXABVCCharString@@@Z); evidence: RTTI vtable slot + PDB name set.
