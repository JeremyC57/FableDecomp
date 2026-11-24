// Leaf: ?GetObjectTransform@CEngineInternalPrimitiveStaticMesh@@UBE_NAAVCMatrix3x4@@@Z
class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x6c];
    CMatrix3x4 transform;
    virtual bool GetObjectTransform(class CMatrix3x4&) const;
};

bool CEngineInternalPrimitiveStaticMesh::GetObjectTransform(class CMatrix3x4& out) const
{
    CEngineInternalPrimitiveStaticMesh* mesh = const_cast<CEngineInternalPrimitiveStaticMesh*>(this);
    out = mesh->transform;
    return true;
}
// Flatten route (flatten_pilot v6, NF335): CEngineInternalPrimitiveStaticMesh_GetObjectTransform -> CEngineInternalPrimitiveStaticMesh::GetObjectTransform, the RTTI-placed PDB member; class model: the row's local class model.
