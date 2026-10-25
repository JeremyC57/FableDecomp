// CPhysicsMesh::GetNoTriangles() const -- the number of triangles in the physics mesh
// Leaf: ?GetNoTriangles@CPhysicsMesh@@QBEJXZ
// Retail: Fable.exe @ 0x00A6C560, 23 bytes. PDB ?GetNoTriangles@CPhysicsMesh@@QBEJXZ
// Label correction: the manifest row "CWorldMap::DrawGetNoMaps" is a propagation. The only caller,
// CIPhysicsFacesSegmentBase::GetCollidedWithTriangles(const CPhysicsMesh*, ...) 0x00A86EA0, calls it on its mesh
// argument and sizes a scratch vector with the result; the body divides the 6-byte C3DTriangle2 array
// (Ego_r.pdb: ushort VertexIndex[3]) TotalFaceIndices, which sits at retail +0x30 (PDB +0x38: the two CVectorMap
// members ahead of it carry 12-byte retail vectors; the landed CalculateCentreOfGravity 0x00A6C890 puts TotalPoints at
// +0x3C). No engine header exists for CPhysicsMesh.
// Shape: the signed pointer difference /6 (`imul 2AAAAAABh`, no shift) returned as a long.
class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];
};

template <class T> class CArray {               // retail CArray<T>: first / last / end (12 bytes)
public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
};

class CPhysicsMesh {
public:
    virtual ~CPhysicsMesh();
    unsigned char         _pad04[0x2c];
    CArray<C3DTriangle2>  TotalFaceIndices;     // +0x30 (PDB +0x38)
    long GetNoTriangles() const;
};

long CPhysicsMesh::GetNoTriangles() const
{
    return TotalFaceIndices.size();
}