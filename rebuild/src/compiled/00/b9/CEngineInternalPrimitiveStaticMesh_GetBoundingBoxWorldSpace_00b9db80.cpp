// CEngineInternalPrimitiveStaticMesh::GetBoundingBoxWorldSpace(C3DBoundingBox&) -- the mesh's local bounding box (from the mesh info at +0x50, offset +0x10) transformed by the primitive's object transform at +0x70; always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9DB80, length 76 bytes (80 with padding).
// Callees: C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox&, const CMatrix3x4&) @ 0x00ADE080 (on the argument; the local copy and the matrix pushed right to
// left). Shape: byte-identical to CEngineInternalPrimitive2DStaticMesh::GetBoundingBoxWorldSpace @ 0x00B9E490 (landed): the six words of the local box are copied
// one by one through edx (the info address re-based with `add eax,10h`), the matrix address is `add ecx,70h`; `mov al,1` is the bool.
// engine/CEngineInternalPrimitiveStaticMesh.h (PDB) has ObjectTransform at +0x70; the mesh info pointer at +0x50 (C3DMeshInfo, bounding box at +0x10) is modelled locally.
struct CMatrix3x4 {
    float m_rows[12];
};

struct C3DBoundingBox {
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

struct C3DMeshInfo_00b9db80 {
    unsigned char  _pad[0x10];
    C3DBoundingBox m_boundingBox;                  // +0x10
};

struct CEngineInternalPrimitiveStaticMesh {
    unsigned char       _pad[0x50];
    C3DMeshInfo_00b9db80* m_pMeshInfo;             // +0x50
    unsigned char       _pad54[0x1c];
    CMatrix3x4          ObjectTransform;           // +0x70 (PDB)
    bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);
};

bool CEngineInternalPrimitiveStaticMesh::GetBoundingBoxWorldSpace(C3DBoundingBox& box)
{
    C3DBoundingBox local(m_pMeshInfo->m_boundingBox);
    box.InitialiseFromOBB(local, ObjectTransform);
    return true;
}