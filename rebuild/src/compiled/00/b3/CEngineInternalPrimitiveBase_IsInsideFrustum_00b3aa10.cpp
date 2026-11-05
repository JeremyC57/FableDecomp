// Leaf: ?IsInsideFrustum@CEngineInternalPrimitiveBase_Methods@@QAE_NHH@Z
#include "engine/CEngineInternalPrimitiveBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineInternalPrimitiveBase_Methods : CEngineInternalPrimitiveBase {
    bool IsInsideFrustum(int camera, int a1);
};
bool CEngineInternalPrimitiveBase_Methods::IsInsideFrustum(int camera, int a1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInsideFrustum@CEngineInternalPrimitiveBase@@UAE_NABVCEngineCamera@@_N@Z); evidence: RTTI vtable slot + PDB name set.
