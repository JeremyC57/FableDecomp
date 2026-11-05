// Constant-false predicate: `xor al,al; ret`. __fastcall this=ecx, no args.
// Leaf: ?UnloadResource@CResource_Methods@@QAE_NXZ
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    bool UnloadResource();
};
bool CResource_Methods::UnloadResource() { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?UnloadResource@CResource@@UAE_NXZ); evidence: RTTI vtable slot + PDB name set.
