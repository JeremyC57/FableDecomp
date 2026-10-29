// C3DMeshObject::C3DMeshObject(C3DMeshInfo*, const C3DVector&, const CRightHandedSet&, float) -- CBase(), empty Mesh pointer, Scale, then SetMesh + SetPositionMatrix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36E70, oracle length 72 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (landed), C3DMeshObject::SetMesh @ 0x00A36010,
//          C3DMeshObject::SetPositionMatrix @ 0x00A357B0. Vtable 0x01239860 = C3DMeshObject (rtti-vtables.tsv).
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; mov ecx,[esp+8] ; xor eax,eax ; mov [esi],offset vftable ;
//                mov [esi+8],eax ; mov [esi+0Ch],eax ; mov [esi+40h],al ; mov eax,[esp+14h] ; push ecx ; mov ecx,esi ;
//                mov [esi+74h],eax ; call SetMesh ; mov edx,[esp+14h] ; mov eax,[esp+10h] ; mov ecx,[esp+0Ch] ;
//                push edx ; push eax ; push ecx ; mov ecx,esi ; call SetPositionMatrix ; mov eax,esi ; pop esi ; ret 10h
// Layout from the PDB (struct_layouts_egor.tsv, C3DMeshObject = 120 bytes): +4 MeshInfo, +8 Mesh
// (CCountedPointer<C3DMeshStats const>, zeroed by its inline constructor), +0x10 PositionMatrix,
// +0x40 InversePositionMatrixSetup (false), +0x44 InversePositionMatrix, +0x74 Scale. MeshInfo and the
// matrices are left to SetMesh / SetPositionMatrix. The scale is forwarded as the raw 4-byte argument.
class C3DMeshInfo;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CMatrix3x4 { public: float m[12]; };

class CBaseClass {
public:
    virtual void v00();
    CBaseClass();                               // 0x0099A2D0
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    void* m_pCount;                        // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class C3DMeshObject : public CBaseClass { public:
    virtual void v00();                    // own vtable (0x01239860)
    C3DMeshInfo*    MeshInfo;              // +0x4
    CCountedPointer Mesh;                  // +0x8
    CMatrix3x4      PositionMatrix;        // +0x10
    bool            InversePositionMatrixSetup; // +0x40
    CMatrix3x4      InversePositionMatrix; // +0x44
    float           Scale;                 // +0x74

    C3DMeshObject(C3DMeshInfo* pMeshInfo, const C3DVector& position, const CRightHandedSet& orientation, float scale);
    void SetMesh(C3DMeshInfo* pMeshInfo);                                                        // 0x00A36010
    void SetPositionMatrix(const C3DVector& position, const CRightHandedSet& orientation, float scale); // 0x00A357B0
};

C3DMeshObject::C3DMeshObject(C3DMeshInfo* pMeshInfo, const C3DVector& position, const CRightHandedSet& orientation, float scale)
    : CBaseClass(), Mesh(), InversePositionMatrixSetup(false), Scale(scale)
{
    SetMesh(pMeshInfo);
    SetPositionMatrix(position, orientation, scale);
}