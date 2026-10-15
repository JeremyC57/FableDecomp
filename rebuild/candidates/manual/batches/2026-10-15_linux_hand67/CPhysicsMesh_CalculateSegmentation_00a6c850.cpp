// CPhysicsMesh::CalculateSegmentation() -- with more than 60 triangles, build the face segmentation tree (0x00A86CD0) and store it in the counted pointer at +0xB4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6C850, length 55 bytes.
// Callees: the static segment factory `CIPhysicsFacesSegmentBase::Create(const CArray<C3DTriangle2>&,
// const CArray<C3DVector>&)` @ 0x00A86CD0 (`__fastcall`: triangles in ecx, vertices in edx),
// CCountedPointer<const CIPhysicsFacesSegmentBase>::Reset(CIPhysicsFacesSegmentBase*) @ 0x00A6E9A0.
// Shape: the triangle array (+0x30) holds 6-byte C3DTriangle2 records (three 16-bit indices): the
// element count is the byte difference times 0x2AAAAAAB with no extra shift; `jbe` = an unsigned
// size(); vertices follow at +0x3C (a retail CArray is first/last/end, 12 bytes). Member names from
// Ego_r.pdb (TotalFaceIndices, TotalPoints, RootSegment): retail keeps the PDB order with 12-byte
// containers, so the PDB +0x38/+0x48/+0xD0 sit at +0x30/+0x3C/+0xB4 here.
struct C3DTriangle2 { unsigned short m_index[3]; };
struct C3DVector { float x, y, z; };
template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
};
struct CIPhysicsFacesSegmentBase {
    static CIPhysicsFacesSegmentBase* __fastcall Create(const CArray<C3DTriangle2>& triangles, const CArray<C3DVector>& vertices);   // 0x00A86CD0
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    void* m_pBlock;
    void Reset(T* object);                         // 0x00A6E9A0
};

struct CPhysicsMesh {
    unsigned char                     _pad00[0x30];
    CArray<C3DTriangle2>              TotalFaceIndices;   // +0x30
    CArray<C3DVector>                 TotalPoints;        // +0x3c
    unsigned char                     _pad48[0xb4 - 0x48];
    CCountedPointer<CIPhysicsFacesSegmentBase> RootSegment;   // +0xb4
    void CalculateSegmentation();
};

void CPhysicsMesh::CalculateSegmentation()
{
    if (TotalFaceIndices.size() > 60)
        RootSegment.Reset(CIPhysicsFacesSegmentBase::Create(TotalFaceIndices, TotalPoints));
}
