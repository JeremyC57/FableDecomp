class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x70];
    CMatrix3x4 frameEnd;
    bool GetTransformFrameEnd(CMatrix3x4 &out);
};

bool CEngineInternalPrimitiveStaticMesh::GetTransformFrameEnd(CMatrix3x4 &out)
{
    out = this->frameEnd;
    return true;
}