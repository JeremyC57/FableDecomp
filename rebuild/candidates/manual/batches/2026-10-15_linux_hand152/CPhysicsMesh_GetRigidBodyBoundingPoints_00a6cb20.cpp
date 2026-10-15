// CPhysicsMesh::GetRigidBodyBoundingPoints(CArray<C3DVector>& points) const -- copy the mesh points and make them relative to the centre of gravity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6CB20, length 127 bytes (+ int3 padding).
// Leaf: GetRigidBodyBoundingPoints
// Callees: CArray<C3DVector>::operator=(const CArray<C3DVector>&) @ 0x0075B8D0.
// Layout: Ego_r.pdb CPhysicsMesh with the retail 12-byte CArray: TotalPoints +0x3c, CentreOfGravity +0xa8 (PDB +0xc4).
// Shape: the output array kept in ebx; the element count (`/12` via 0x2AAAAAAB) is re-derived from first/last every pass
// with an unsigned index (`jb`), the loop entered only for a non-empty array; each element is reached as
// `*(begin() + i)` (the STLport operator[] form) and the centre subtracted component by component; `ret 4`.
struct C3DVector {
    float x, y, z;
    C3DVector& operator-=(const C3DVector& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
};

template <class T>
struct CArray {                                    // retail CArray<T>: first / last / end
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    T& operator[](unsigned long index) { return *(begin() + index); }
    CArray& operator=(const CArray& other);        // 0x0075B8D0 for C3DVector
};

struct CPhysicsMesh {
    unsigned char      _pad00[0x3c];
    CArray<C3DVector>  TotalPoints;                // +0x3c
    unsigned char      _pad48[0xa8 - 0x48];
    C3DVector          CentreOfGravity;            // +0xa8
    void GetRigidBodyBoundingPoints(CArray<C3DVector>& points) const;
};

void CPhysicsMesh::GetRigidBodyBoundingPoints(CArray<C3DVector>& points) const
{
    points = TotalPoints;
    for (unsigned long i = 0; i < points.size(); ++i)
        points[i] -= CentreOfGravity;
}
