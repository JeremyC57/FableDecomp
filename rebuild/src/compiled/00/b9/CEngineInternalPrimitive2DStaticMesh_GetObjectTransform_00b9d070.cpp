// Leaf: ?GetObjectTransform@CEngineInternalPrimitive2DStaticMesh@@UBE_NAAVCMatrix3x4@@@Z
class CMatrix3x4 { public:
    float m[12];
};

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x6c];
    CMatrix3x4 transform;
    virtual bool GetObjectTransform(class CMatrix3x4&) const;
};

bool CEngineInternalPrimitive2DStaticMesh::GetObjectTransform(class CMatrix3x4& out) const
{
    CEngineInternalPrimitive2DStaticMesh* mesh = const_cast<CEngineInternalPrimitive2DStaticMesh*>(this);
    out = mesh->transform;
    return true;
}
// Flatten route (flatten_pilot v6, NF335): CEngineInternalPrimitive2DStaticMesh_GetObjectTransform -> CEngineInternalPrimitive2DStaticMesh::GetObjectTransform, the RTTI-placed PDB member; class model: the row's local class model.
