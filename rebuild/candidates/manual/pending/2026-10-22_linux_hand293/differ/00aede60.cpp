// Grade: DIFFER(796v796) -- x87 commutative operand order only: VC7.1 canonicalises every fmul pair in the inlined SetInverseNoTranslate (and the Right.Z cross term) to the mirror of retail regardless of source order, so the det terms and cofactor products load the other operand first; size, frame, stores and callees match
// NParticleEngine::CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& orientation) -- take a new start/end orientation: copy the interpolator, make both right-handed sets perpendicular, rebuild Transform from the start orientation (keeping its translation), InverseTransform as its rotation-only inverse, and EndTransform from the end orientation (keeping its translation)
// Leaf: SetOrientation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEDE60, 796 bytes (+ int3 padding), `ret 4`.
// Callees: C3DOrientationInterpolator::operator= 0x006B0E00, CRightHandedSet::MakePerpendicular 0x00A22E10 (x2).
//   1.0f 0x0122DED8. CMatrix3x4::Set(const CRightHandedSet&, const C3DVector&), GetTranslation and SetInverseNoTranslate
//   are inline.
// Layout: Ego_r.pdb CParticleEmitter minus 0x0c (the landed constructor 0x00AF0380): OrientationInterpolator +0x60
//   (StartOrientation +0x64, EndOrientation +0x7c; CRightHandedSet = Up, Forward), Transform +0xb0, InverseTransform
//   +0xe0, EndTransform +0x110 (CMatrix3x4 rows Right, Forward, Up, Translation).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator^(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
};
struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
    void MakePerpendicular();                      // 0x00A22E10
};
struct C3DOrientationInterpolator {
    void*           __vftable;                     // +0x00
    CRightHandedSet StartOrientation;              // +0x04
    CRightHandedSet EndOrientation;                // +0x1c
    C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& o);   // 0x006B0E00
};
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    C3DVector GetTranslation() const { return C3DVector(E41, E42, E43); }
    __forceinline void Set(const C3DVector& right, const C3DVector& forward, const C3DVector& up, const C3DVector& pos)
    {
        E11 = right.X; E12 = right.Y; E13 = right.Z;
        E21 = forward.X; E22 = forward.Y; E23 = forward.Z;
        E31 = up.X; E32 = up.Y; E33 = up.Z;
        E41 = pos.X; E42 = pos.Y; E43 = pos.Z;
    }
    __forceinline void Set(const CRightHandedSet& o, const C3DVector& pos) { Set(o.Forward ^ o.Up, o.Forward, o.Up, pos); }
    __forceinline void SetInverseNoTranslate(const CMatrix3x4& m)
    {
        float a11 = m.E22 * m.E33 - m.E32 * m.E23;
        float det = (m.E32 * m.E13 - m.E33 * m.E12) * m.E21 + (m.E23 * m.E12 - m.E22 * m.E13) * m.E31 + a11 * m.E11;
        float inv = 1.0f / det;
        E11 = a11 * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E32 * m.E21 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E11 * m.E33 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E11 * m.E32) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E11 * m.E23) * inv;
        E41 = E42 = E43 = 0;
        E33 = (m.E11 * m.E22 - m.E21 * m.E12) * inv;
    }
};
namespace NParticleEngine {
class CParticleEmitter {
public:
    unsigned char              _state00[0x60];
    C3DOrientationInterpolator OrientationInterpolator;   // +0x60
    unsigned char              _state94[0x1c];
    CMatrix3x4                 Transform;                 // +0xb0
    CMatrix3x4                 InverseTransform;          // +0xe0
    CMatrix3x4                 EndTransform;              // +0x110
    void SetOrientation(const C3DOrientationInterpolator& orientation);
};

void CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& orientation)
{
    OrientationInterpolator = orientation;
    OrientationInterpolator.StartOrientation.MakePerpendicular();
    OrientationInterpolator.EndOrientation.MakePerpendicular();
    Transform.Set(OrientationInterpolator.StartOrientation, Transform.GetTranslation());
    InverseTransform.SetInverseNoTranslate(Transform);
    EndTransform.Set(OrientationInterpolator.EndOrientation, EndTransform.GetTranslation());
}
}
