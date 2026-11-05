// Leaf: ?CountReferences@CVertexShader_Methods@@QAEKXZ
#include "engine/CVertexShader.h"  // retyped onto the PDB layout; byte parity re-verified
struct CVertexShaderData { char pad[0x10]; unsigned long f; };
struct CVertexShader_Methods : CVertexShader {
    unsigned long CountReferences();
};
unsigned long CVertexShader_Methods::CountReferences(){ return this->Data->f; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?CountReferences@CVertexShader@@EAEKXZ); evidence: RTTI vtable slot + PDB name set.
