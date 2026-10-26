// CEngineInternalPrimitive2DStaticMesh::GetBoundingBoxWorldSpace(C3DBoundingBox&) -- the mesh's local bounding box (from the mesh record at +0x50, offset +0x10) transformed by the primitive's matrix at +0x70; always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9E490, length 73 bytes.
// Callees: C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox&, const CMatrix3x4&) @ 0x00ADE080 (on the argument; the local copy and the matrix pushed right to
// left). Shape: the six words of the local box are copied one by one through edx (the record address re-based with `add eax,10h`), the matrix address is
// `add ecx,70h`; `mov al,1` is the bool. engine/CEngineInternalPrimitive2DStaticMesh.h (PDB) has the matrix at +0x70; the mesh record pointer at +0x50 is
// modelled locally.
class CMatrix3x4 { public:
    float m_rows[12];
};

class C3DBoundingBox { public:
    float Min[3];
    float Max[3];
    C3DBoundingBox() { }
    C3DBoundingBox(const C3DBoundingBox& o)
    {
        Min[0] = o.Min[0]; Min[1] = o.Min[1]; Min[2] = o.Min[2];
        Max[0] = o.Max[0]; Max[1] = o.Max[1]; Max[2] = o.Max[2];
    }
    void InitialiseFromOBB(const C3DBoundingBox& box, const CMatrix3x4& transform);   // 0x00ADE080
};

struct SMeshRecord_00b9e490 {
    unsigned char  _pad[0x10];
    C3DBoundingBox m_localBox;                     // +0x10
};

struct CEngineInternalPrimitive2DStaticMesh {
    unsigned char       _pad[0x50];
    SMeshRecord_00b9e490* m_pMesh;                 // +0x50
    unsigned char       _pad54[0x1c];
    CMatrix3x4          m_transform;               // +0x70
    bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);
};

bool CEngineInternalPrimitive2DStaticMesh::GetBoundingBoxWorldSpace(C3DBoundingBox& box)
{
    C3DBoundingBox local(m_pMesh->m_localBox);
    box.InitialiseFromOBB(local, m_transform);
    return true;
}