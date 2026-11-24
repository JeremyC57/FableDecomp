// Leaf: ?SetObjectTransform@CEngineInternalPrimitiveStaticMesh@@UAE_NABVCMatrix3x4@@@Z
class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x6c];
    CMatrix3x4 transform;
    virtual bool SetObjectTransform(const class CMatrix3x4&);
};

bool CEngineInternalPrimitiveStaticMesh::SetObjectTransform(const class CMatrix3x4& mat)
{
    CEngineInternalPrimitiveStaticMesh* mesh = this;
    mesh->transform = mat;
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CEngineInternalPrimitiveStaticMesh_SetObjectTransform -> CEngineInternalPrimitiveStaticMesh::SetObjectTransform, the RTTI-placed PDB member; class model: the row's local class model.
