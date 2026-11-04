// CPhysicsMesh::GetNoTriangles() const -- the number of triangles in the physics mesh
// Leaf: ?GetNoTriangles@CPhysicsMesh@@QBEJXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
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

#include "engine/CArray.h"

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