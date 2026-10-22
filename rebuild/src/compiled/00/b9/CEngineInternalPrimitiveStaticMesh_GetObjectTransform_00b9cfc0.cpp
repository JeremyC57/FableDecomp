struct CMatrix3x4 { float m[12]; };

struct CEngineInternalPrimitiveStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitiveStaticMesh_GetObjectTransform(
    CEngineInternalPrimitiveStaticMesh* mesh, int /*edx*/, CMatrix3x4& out)
{
    out = mesh->transform;
    return true;
}