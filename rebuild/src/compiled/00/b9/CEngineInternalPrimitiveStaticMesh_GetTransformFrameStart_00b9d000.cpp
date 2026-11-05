// Leaf: ?CEngineInternalPrimitiveStaticMesh_GetTransformFrameStart@@YI_NPAVCEngineInternalPrimitiveStaticMesh@@HAAVCMatrix3x4@@@Z
class CMatrix3x4 { public:
    float m[12];
};

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x70];
    CMatrix3x4 frame; // at +0x70
};

bool __fastcall CEngineInternalPrimitiveStaticMesh_GetTransformFrameStart(
    CEngineInternalPrimitiveStaticMesh* mesh, int /*edx*/, CMatrix3x4& out)
{
    out = mesh->frame;
    return true;
}