// Leaf: ?Update@CEngineInternalPrimitiveStaticMeshFromFile_Methods@@QAEXHH@Z
#include "engine/CEngineInternalPrimitiveStaticMeshFromFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineInternalPrimitiveStaticMeshFromFile_Methods : CEngineInternalPrimitiveStaticMeshFromFile {
    void Update(int primitive, int b);
};
void CEngineInternalPrimitiveStaticMeshFromFile_Methods::Update(int primitive, int b) { this->fc = b; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitiveStaticMeshFromFile@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
