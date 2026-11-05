// Leaf: ?CEngineInternalPrimitiveStaticMesh_SetObjectTransform@@YI_NPAVCEngineInternalPrimitiveStaticMesh@@HABVCMatrix3x4@@@Z
class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitiveStaticMesh_SetObjectTransform(CEngineInternalPrimitiveStaticMesh* mesh, int /*edx*/, const CMatrix3x4& mat)
{
    mesh->transform = mat;
    return true;
}