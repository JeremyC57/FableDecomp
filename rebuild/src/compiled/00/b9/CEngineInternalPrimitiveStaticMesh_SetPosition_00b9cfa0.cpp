// CEngineInternalPrimitiveStaticMesh::SetPosition @ 00b9cfa0
class CEnginePrimitivePosition { public:
    char pad[0xc];
    int payload; // member at +0xc
};

// callee at 0xbebaa0 is CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (static __fastcall, Ego_r / FableWin; naming wave 11):
//   matrix -> ecx (ObjectTransform, this+0x70)
//   position -> edx (the CEnginePrimitivePosition's position, +0)
//   stack -> (orientation = the position's right-handed set +0xc, scale = this+0x58)
// It cleans its own 2 stack args (8 bytes); this function cleans its 1 ref arg (ret 4).
class CMatrix3x4;
class C3DVector;
class CRightHandedSet;
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00BEBAA0 (Ego_r / FableWin)
};

class CEngineInternalPrimitiveStaticMesh { public:
    char pad0[0x58];
    float field58;               // +0x58 (CEngineInternalPrimitiveMeshBase::Scale)
    char pad1[0x70 - 0x58 - 4];
    char sub;                    // +0x70 (embedded sub-object)
};

// __fastcall method: 'this' in ecx, the CEnginePrimitivePosition reference on the
// stack. Modeled as __fastcall(self, dummy-edx, stack-ref) so the ref stays on the
// stack and the function ends with ret 4.
void __fastcall CEngineInternalPrimitiveStaticMesh_SetPosition(
        CEngineInternalPrimitiveStaticMesh* self,
        int /*edx*/,
        const CEnginePrimitivePosition& pos)
{
    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(
        *(CMatrix3x4*)&self->sub, *(const C3DVector*)&pos, *(const CRightHandedSet*)&pos.payload, self->field58);
}