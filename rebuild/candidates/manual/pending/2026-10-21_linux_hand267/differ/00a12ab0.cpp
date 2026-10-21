// Grade: DIFFER(539v468) -- sketch. the inline affine inverse (adjugate/det of PositionMatrix, then the translation row) is reproduced term for term but VC7.1 keeps a full returned-matrix temporary (frame 0x70 v 0x40: retail stores only the matrix elements it later reloads, the rest stay on the x87 stack) and picks a different fld/fmul operand order per product (it varies between these four sibling functions, so it is not a source-order or address rule). One sketch attempt each (budget rows); next: model the inverse as statement-by-statement locals rather than a returned CMatrix3x4, then probe the per-product operand order
// CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume(long volume, const C3DVector& centre, float radius) -- rough fraction of a world-space sphere inside one of the mesh's containment volumes (sphere taken into mesh space, radius scaled by OO_Scale), holding a counted reference on the mesh for the query
// Leaf: ?PeekRoughProportionOfSphereContainedInVolume@CPhysicsMeshObject@
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
    // Inline affine inverse: adjugate of the 3x3 rotation over the determinant, then the translation row.
    __forceinline CMatrix3x4 GetInverse() const
    {
        CMatrix3x4 r;
        float c0 = E13 * E32 - E12 * E33;
        float c1 = E12 * E23 - E13 * E22;
        float c2 = E33 * E22 - E23 * E32;
        float invDet = 1.0f / (E11 * c2 + E31 * c1 + E21 * c0);
        r.E11 = c2 * invDet;
        r.E21 = (E31 * E23 - E33 * E21) * invDet;
        r.E31 = (E21 * E32 - E31 * E22) * invDet;
        r.E12 = c0 * invDet;
        r.E22 = (E11 * E33 - E31 * E13) * invDet;
        r.E32 = (E12 * E31 - E11 * E32) * invDet;
        r.E13 = c1 * invDet;
        r.E23 = (E13 * E21 - E11 * E23) * invDet;
        r.E33 = (E11 * E22 - E12 * E21) * invDet;
        r.E41 = -(r.E11 * E41 + r.E31 * E43 + r.E21 * E42);
        r.E42 = -(r.E12 * E41 + r.E32 * E43 + r.E22 * E42);
        r.E43 = -(r.E13 * E41 + r.E33 * E43 + r.E23 * E42);
        return r;
    }
};
__forceinline C3DVector operator*(const C3DVector& p, const CMatrix3x4& m)
{
    return C3DVector(m.E31 * p.Z + m.E21 * p.Y + m.E11 * p.X + m.E41,
                     m.E32 * p.Z + m.E22 * p.Y + m.E12 * p.X + m.E42,
                     m.E33 * p.Z + m.E23 * p.Y + m.E13 * p.X + m.E43);
}
struct CPhysicsMesh {
    virtual ~CPhysicsMesh();
    virtual void Release();                        // slot 1: called when the count drops to zero
    long RefCount;                                 // +0x04
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
    static bool IsSphereInLocalBoundingBox(const CPhysicsMeshObject* obj, const C3DVector& centre, float radius);   // 0x00A17690 (shape guessed)
};

float CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume(long volume, const C3DVector& centre, float radius) const
{
    C3DVector local = centre * PositionMatrix.GetInverse();
    radius *= OO_Scale;
    CIVCountedPointer<const CPhysicsMesh> mesh(Mesh);
    return mesh->PeekRoughProportionOfSphereContainedInVolume(volume, local, radius);
}
