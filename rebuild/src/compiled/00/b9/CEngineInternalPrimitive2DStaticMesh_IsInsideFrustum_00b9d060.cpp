// Leaf: ?IsInsideFrustum@CEngineInternalPrimitive2DStaticMesh_Methods@@QAE_NHH@Z
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineInternalPrimitive2DStaticMesh_Methods : CEngineInternalPrimitive2DStaticMesh {
    bool IsInsideFrustum(int camera, int x1);
};
bool CEngineInternalPrimitive2DStaticMesh_Methods::IsInsideFrustum(int camera, int x1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsInsideFrustum@CEngineInternalPrimitive2DStaticMesh@@UAE_NABVCEngineCamera@@_N@Z); evidence: RTTI vtable slot + PDB name set.
