// callee at 0xbebaa0: CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (static __fastcall, Ego_r / FableWin; naming wave 11):
// ecx = matrix, edx = EndPosition self+0x94, stack: EndOrientation self+0xbc, Scale +0x58
class CMatrix3x4;
class C3DVector;
class CRightHandedSet;
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00BEBAA0 (Ego_r / FableWin)
};

struct Mesh {
    char pad0[0x58];
    float field58;      // +0x58 (Scale)
    char pad1[0x94 - 0x58 - 4];
    char field94[0x100]; // +0x94 region; +0xbc lands inside
};

// self in ecx, matrix ref on stack -> ret 4
bool __fastcall GetTransformFrameEnd(Mesh* self, void* /*edx*/, CMatrix3x4& matrix)
{
    char* base = (char*)self;
    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(matrix, *(const C3DVector*)(base + 0x94), *(const CRightHandedSet*)(base + 0xbc), self->field58);
    return true;
}