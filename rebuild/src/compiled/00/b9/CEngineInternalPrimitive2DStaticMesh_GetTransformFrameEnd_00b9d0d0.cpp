// Leaf: ?CEngineInternalPrimitive2DStaticMesh_GetTransformFrameEnd@@YI_NPAUCEngineInternalPrimitive2DStaticMesh@@HAAVCMatrix3x4@@@Z
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"  // retyped onto the PDB layout; byte parity re-verified

class CMatrix3x4 { public:
    float m[12];
};

bool __fastcall CEngineInternalPrimitive2DStaticMesh_GetTransformFrameEnd(
    CEngineInternalPrimitive2DStaticMesh* mesh, int /*edx*/, CMatrix3x4& out)
{
    out = *(CMatrix3x4*)&mesh->transform;
    return true;
}
