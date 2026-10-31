// CPhysicsMeshObject::IsSpherePartContainedInBoundingBox(const C3DVector& centre, float radius) -- whether part of a world-space sphere lies inside the mesh's bounding box (sphere taken into mesh space through the inverse position matrix, radius scaled by OO_Scale)
// Grade: DIFFER(434v428) -- with the proven callee CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float, const C3DVector&, const C3DVector&) const the body is 6 bytes long; the residue is the inline 3x4 affine-inverse x87 schedule (invDet homing, cofactor order) -- the lane gap of GOTCHAS chunk 293
// Leaf: ?IsSpherePartContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@M@Z
// Chunk 615: the parked 428v428 file reached equal length only with a guessed callee shape (a static
//   IsSphereInLocalBoundingBox(obj, centre, radius)); manifest + reloc_bindings name 0x00A17690
//   ?HasBoundingBoxIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z, called as Mesh->HasBoundingBoxIntersectedFromTo(r, p, p).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    CMatrix3x4() {}
    __forceinline void SetInverse(const CMatrix3x4& m)
    {
        float c0 = m.E32 * m.E13 - m.E12 * m.E33;
        float c1 = m.E12 * m.E23 - m.E22 * m.E13;
        float c2 = m.E22 * m.E33 - m.E32 * m.E23;
        float invDet = 1.0f / (c2 * m.E11 + c0 * m.E21 + c1 * m.E31);
        E11 = c2 * invDet;
        E21 = (m.E31 * m.E23 - m.E21 * m.E33) * invDet;
        E31 = (m.E32 * m.E21 - m.E31 * m.E22) * invDet;
        E12 = c0 * invDet;
        E22 = (m.E11 * m.E33 - m.E31 * m.E13) * invDet;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * invDet;
        E13 = c1 * invDet;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * invDet;
        E33 = (m.E22 * m.E11 - m.E12 * m.E21) * invDet;
        E41 = -(E31 * m.E43 + E21 * m.E42 + E11 * m.E41);
        E42 = -(E32 * m.E43 + E22 * m.E42 + E12 * m.E41);
        E43 = -(E33 * m.E43 + E23 * m.E42 + E13 * m.E41);
    }
};
__forceinline C3DVector operator*(const C3DVector& p, const CMatrix3x4& m)
{
    return C3DVector(m.E11 * p.X + m.E31 * p.Z + m.E21 * p.Y + m.E41,
                     m.E12 * p.X + m.E32 * p.Z + m.E22 * p.Y + m.E42,
                     m.E13 * p.X + m.E33 * p.Z + m.E23 * p.Y + m.E43);
}
struct CPhysicsMesh {
    virtual ~CPhysicsMesh();
    virtual void Release();                        // slot 1: called when the count drops to zero
    long RefCount;                                 // +0x04
    bool HasBoundingBoxIntersectedFromTo(float radius, const C3DVector& from, const C3DVector& to) const;   // 0x00A17690
    bool IsInContainmentVolume(long volume, const C3DVector& centre, float radius) const;              // 0x00A6CA20
    float PeekRoughProportionOfSphereContainedInVolume(long volume, const C3DVector& centre, float radius) const;  // 0x00A6CA80
};
template<class T> struct CIVCountedPointer {
    T* Ptr;
    CIVCountedPointer(const CIVCountedPointer& o) : Ptr(o.Ptr) { if (Ptr) ++const_cast<CPhysicsMesh*>(Ptr)->RefCount; }
    ~CIVCountedPointer() { if (Ptr && --const_cast<CPhysicsMesh*>(Ptr)->RefCount == 0) const_cast<CPhysicsMesh*>(Ptr)->Release(); }
    T* operator->() const { return Ptr; }
};
struct CPhysicsMeshObject {
    virtual ~CPhysicsMeshObject();
    CIVCountedPointer<const CPhysicsMesh> Mesh;    // +0x04
    CMatrix3x4 PositionMatrix;                     // +0x08
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    bool IsSpherePartContainedInBoundingBox(const C3DVector& centre, float radius) const;
    bool IsInContainmentVolume(long volume, const C3DVector& centre, float radius) const;
    float PeekRoughProportionOfSphereContainedInVolume(long volume, const C3DVector& centre, float radius) const;
};

bool CPhysicsMeshObject::IsSpherePartContainedInBoundingBox(const C3DVector& centre, float radius) const
{
    float localRadius = radius * OO_Scale;
    CMatrix3x4 inverse;
    inverse.SetInverse(PositionMatrix);
    C3DVector local = centre * inverse;
    return Mesh->HasBoundingBoxIntersectedFromTo(localRadius, local, local);
}
