// CPhysicsMeshObject::SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale) -- build the object's position matrix from an up/forward axis pair, a position and a uniform scale: rows -(forward x up), -forward, up, scaled, translated to the position; remember the scale and its reciprocal
// Grade: DIFFER(329v338) -- forward copied before up and the cross product taken from the copies (128 diff lines v 149);
//   left: x87 schedule of the axis copies
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A114B0, length 338 bytes (+ int3 padding), `ret 0Ch`.
// Identity: Ego_r.pdb ?SetPositionMatrix@CPhysicsMeshObject@@QAEXABVC3DVector@@ABVCRightHandedSet@@M@Z.
// Data: 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CPhysicsMeshObject (0x40 bytes): Mesh +4, PositionMatrix (CMatrix3x4, rows of three floats) +8,
// Scale +0x38, OO_Scale +0x3C; CRightHandedSet Up +0, Forward +0xC; CMatrix3x4 as the landed
// C3DBoundingBox::CalculateMatrix 0x00ADEBC0.
// Shape: the matrix is reset to identity first (the dead stores stay), the position is added, the three rows are
// written from the cross product and the axis copies, then every rotation element is scaled.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - b.X * a.Y);
}
struct CRightHandedSet {
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0c
};
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
struct CPhysicsMesh;
struct CPhysicsMeshObject {
    void*         __vftable;                            // +0x00
    CPhysicsMesh* Mesh;                                 // +0x04 CIVCountedPointer<const CPhysicsMesh>
    CMatrix3x4    PositionMatrix;                       // +0x08
    float         Scale;                                // +0x38
    float         OO_Scale;                             // +0x3c
    void SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale);
};

void CPhysicsMeshObject::SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale)
{
    C3DVector forward = axes.Forward;
    C3DVector up = axes.Up;
    C3DVector right = CrossProduct(forward, up);
    PositionMatrix.E11 = 1.0f; PositionMatrix.E12 = 0.0f; PositionMatrix.E13 = 0.0f;
    PositionMatrix.E21 = 0.0f; PositionMatrix.E22 = 1.0f; PositionMatrix.E23 = 0.0f;
    PositionMatrix.E31 = 0.0f; PositionMatrix.E32 = 0.0f; PositionMatrix.E33 = 1.0f;
    PositionMatrix.E41 = 0.0f; PositionMatrix.E42 = 0.0f; PositionMatrix.E43 = 0.0f;
    C3DVector p = position;
    PositionMatrix.E41 += p.X;
    PositionMatrix.E42 += p.Y;
    PositionMatrix.E43 += p.Z;
    PositionMatrix.E11 = -right.X;
    PositionMatrix.E12 = -right.Y;
    PositionMatrix.E13 = -right.Z;
    PositionMatrix.E21 = -forward.X;
    PositionMatrix.E22 = -forward.Y;
    PositionMatrix.E23 = -forward.Z;
    PositionMatrix.E31 = up.X;
    PositionMatrix.E32 = up.Y;
    PositionMatrix.E33 = up.Z;
    PositionMatrix.E11 *= scale;
    PositionMatrix.E12 *= scale;
    PositionMatrix.E13 *= scale;
    PositionMatrix.E21 *= scale;
    PositionMatrix.E22 *= scale;
    PositionMatrix.E23 *= scale;
    PositionMatrix.E31 *= scale;
    PositionMatrix.E32 *= scale;
    PositionMatrix.E33 *= scale;
    Scale = scale;
    OO_Scale = 1.0f / scale;
}
