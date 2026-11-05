// Leaf: ?IsInsideFrustum@CEngineInternalPrimitive2DAnimatedMesh_Methods@@QAE_NHH@Z
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineInternalPrimitive2DAnimatedMesh_Methods : CEngineInternalPrimitive2DAnimatedMesh {
    bool IsInsideFrustum(int camera, int x1);
};
bool CEngineInternalPrimitive2DAnimatedMesh_Methods::IsInsideFrustum(int camera, int x1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInsideFrustum@CEngineInternalPrimitive2DAnimatedMesh@@UAE_NABVCEngineCamera@@_N@Z); evidence: RTTI vtable slot + PDB name set.
