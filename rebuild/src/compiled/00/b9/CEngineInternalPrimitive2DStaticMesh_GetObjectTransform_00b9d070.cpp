class CMatrix3x4 { public:
    float m[12];
};

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitive2DStaticMesh_GetObjectTransform(
    CEngineInternalPrimitive2DStaticMesh* mesh, int /*edx*/, CMatrix3x4& out)
{
    out = mesh->transform;
    return true;
}