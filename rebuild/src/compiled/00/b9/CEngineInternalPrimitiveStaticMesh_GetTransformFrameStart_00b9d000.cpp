// Leaf: ?GetTransformFrameStart@CEngineInternalPrimitiveStaticMesh@@UBE_NAAVCMatrix3x4@@@Z
class CMatrix3x4 { public:
    float m[12];
};

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x6c];
    CMatrix3x4 frame; // at +0x70
    virtual bool GetTransformFrameStart(class CMatrix3x4&) const;
};

bool CEngineInternalPrimitiveStaticMesh::GetTransformFrameStart(class CMatrix3x4& out) const
{
    CEngineInternalPrimitiveStaticMesh* mesh = const_cast<CEngineInternalPrimitiveStaticMesh*>(this);
    out = mesh->frame;
    return true;
}
// Flatten route (flatten_pilot v6, NF335): CEngineInternalPrimitiveStaticMesh_GetTransformFrameStart -> CEngineInternalPrimitiveStaticMesh::GetTransformFrameStart, the RTTI-placed PDB member; class model: the row's local class model.
