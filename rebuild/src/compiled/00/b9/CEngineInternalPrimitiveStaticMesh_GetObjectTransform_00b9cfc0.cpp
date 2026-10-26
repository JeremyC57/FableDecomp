class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitiveStaticMesh_GetObjectTransform(
    CEngineInternalPrimitiveStaticMesh* mesh, int /*edx*/, CMatrix3x4& out)
{
    out = mesh->transform;
    return true;
}