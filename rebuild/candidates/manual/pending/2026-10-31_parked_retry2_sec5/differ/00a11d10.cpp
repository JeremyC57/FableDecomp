// CPhysicsMeshObject::GetNavigationLevelAt(const C3DVector& pos) -- the mesh's navigation level under a world position (taken into mesh space through the inverse position matrix); null when the mesh has no navigation levels
// Leaf: ?GetNavigationLevelAt@CPhysicsMeshObject@
// Grade: DIFFER(426v422) -- in-place forceinline SetInverse into a named matrix instead of a returned GetInverse
//   temporary (frame now matches size class; 89 diff lines v 125); left: x87 schedule of the cofactors, invDet homing
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
    __forceinline void SetInverse(const CMatrix3x4& m)
    {
        float c0 = m.E13 * m.E32 - m.E12 * m.E33;
        float c1 = m.E12 * m.E23 - m.E13 * m.E22;
        float c2 = m.E33 * m.E22 - m.E23 * m.E32;
        float invDet = 1.0f / (m.E11 * c2 + m.E31 * c1 + m.E21 * c0);
        E11 = c2 * invDet;
        E21 = (m.E31 * m.E23 - m.E33 * m.E21) * invDet;
        E31 = (m.E21 * m.E32 - m.E31 * m.E22) * invDet;
        E12 = c0 * invDet;
        E22 = (m.E11 * m.E33 - m.E31 * m.E13) * invDet;
        E32 = (m.E12 * m.E31 - m.E11 * m.E32) * invDet;
        E13 = c1 * invDet;
        E23 = (m.E13 * m.E21 - m.E11 * m.E23) * invDet;
        E33 = (m.E11 * m.E22 - m.E12 * m.E21) * invDet;
        E41 = -(E11 * m.E41 + E31 * m.E43 + E21 * m.E42);
        E42 = -(E12 * m.E41 + E32 * m.E43 + E22 * m.E42);
        E43 = -(E13 * m.E41 + E33 * m.E43 + E23 * m.E42);
    }
};
__forceinline C3DVector operator*(const C3DVector& p, const CMatrix3x4& m)
{
    return C3DVector(m.E31 * p.Z + m.E21 * p.Y + m.E11 * p.X + m.E41,
                     m.E32 * p.Z + m.E22 * p.Y + m.E12 * p.X + m.E42,
                     m.E33 * p.Z + m.E23 * p.Y + m.E13 * p.X + m.E43);
}
struct CNavigationLevel;
struct CPhysicsMesh {
    unsigned char _pad00[0x54];
    void* NavFirst;                                // +0x54 navigation-level vector begin
    void* NavLast;                                 // +0x58 end
    const CNavigationLevel* GetNavigationLevelAt(const C3DVector& pos) const;   // 0x00A73D10
    bool IsSpherePartContainedInBoundingBox(const C3DVector& centre, float radius) const;
};
struct CPhysicsMeshObject {
    virtual ~CPhysicsMeshObject();
    const CPhysicsMesh* Mesh;                      // +0x04 CIVCountedPointer<const CPhysicsMesh>
    CMatrix3x4 PositionMatrix;                     // +0x08
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    const CNavigationLevel* GetNavigationLevelAt(const C3DVector& pos) const;
};
const CNavigationLevel* CPhysicsMeshObject::GetNavigationLevelAt(const C3DVector& pos) const
{
    if (Mesh->NavFirst == Mesh->NavLast)
        return 0;
    CMatrix3x4 inverse;
    inverse.SetInverse(PositionMatrix);
    return Mesh->GetNavigationLevelAt(pos * inverse);
}
