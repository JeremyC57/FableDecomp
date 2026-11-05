// Empty member hook. __fastcall this=ecx.
// Leaf: ?Validate@CDefClassBase_Methods@@QAEXHH@Z
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase_Methods : CDefClassBase {
    void Validate(int init, int name);
};
void CDefClassBase_Methods::Validate(int init, int name) {}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Validate@CDefClassBase@@UBEXABVCValidateDefinitionInitBase@@ABVCCharString@@@Z); evidence: RTTI vtable slot + PDB name set.
