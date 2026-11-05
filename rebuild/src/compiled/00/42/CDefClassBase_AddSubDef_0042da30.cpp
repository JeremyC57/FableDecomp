// Empty member hook. __fastcall this=ecx.
// Leaf: ?AddSubDef@CDefClassBase_Methods@@QAEXHHH@Z
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase_Methods : CDefClassBase {
    void AddSubDef(int name, int a1, int a2);
};
void CDefClassBase_Methods::AddSubDef(int name, int a1, int a2) {}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?AddSubDef@CDefClassBase@@UAEXABVCCharString@@JJ@Z); evidence: RTTI vtable slot + PDB name set.
