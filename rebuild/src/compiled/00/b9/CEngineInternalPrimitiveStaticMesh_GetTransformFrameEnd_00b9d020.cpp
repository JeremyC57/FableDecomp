// Leaf: ?GetTransformFrameEnd@CEngineInternalPrimitiveStaticMesh@@UBE_NAAVCMatrix3x4@@@Z
class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x6c];
    CMatrix3x4 frameEnd;
    virtual bool GetTransformFrameEnd(CMatrix3x4 &out) const;
};

bool CEngineInternalPrimitiveStaticMesh::GetTransformFrameEnd(CMatrix3x4 &out) const
{
    out = this->frameEnd;
    return true;
}