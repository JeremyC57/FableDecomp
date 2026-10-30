class CMatrix3x4 { public: char pad[64]; };

class CEngineInternalPrimitiveAnimatedMesh { public:
    char pad[0x58];
    float m58;       // +0x58 (Scale)
    char pad2[0x88 - 0x5c];
    char m88[0x1c];  // +0x88
    char mA4[0x1c];  // +0xa4
};

// callee at 0xbebaa0: CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (static __fastcall, Ego_r / FableWin; naming wave 11):
// ecx = matrix arg, edx = StartPosition self+0x88, stack: StartOrientation self+0xa4, Scale +0x58
class C3DVector;
class CRightHandedSet;
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00BEBAA0 (Ego_r / FableWin)
};

bool __fastcall GetTransformFrameStart(CEngineInternalPrimitiveAnimatedMesh* self, void* edxDummy, CMatrix3x4& mtx)
{
    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(mtx, *(const C3DVector*)self->m88, *(const CRightHandedSet*)self->mA4, self->m58);
    return true;
}