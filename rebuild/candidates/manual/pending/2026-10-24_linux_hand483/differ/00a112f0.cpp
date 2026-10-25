// CMatrix3x4::TransformPlane(CPlane& plane) const -- move a plane through the matrix: rotate and renormalise its normal, transform the point normal*distance (rotation plus translation) and rebuild the plane from the new normal and the new point's distance along it
// Grade: DIFFER(316v316) -- x87 stack allocation and frame: ours keeps the copied normal (x, y, z) on the FPU stack and has a 0x24 frame with the renormalised normal in its own slot; retail copies z through eax into a spill slot, spills x into the dead plane-argument slot and y to [esp+8] after the point is scaled, and reuses the rotated temporary for the renormalised normal (0x1c frame)
// Leaf: ?TransformPlane@CMatrix3x4@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A112F0, 309 bytes, thiscall `ret 4`. No direct callers in the image.
// Name: Ego_r.pdb `?TransformPlane@CMatrix3x4@@QBEXAAVCPlane@@@Z` (the chunk label "Matrix_TransformPlane" is descriptive; its
//   "calls GetScaled" is the propagated C3DVector::Normalise 0x00A14440).
// Layout: Ego_r.pdb CMatrix3x4 (0x30) E11..E43 (rotation rows E1x..E3x, translation E41..E43); CPlane Normal +0,
//   Distance +0xC.
// Callees: C3DVector::Normalise @ 0x00A14440 (in place, returns *this in eax), CPlane::Initialise(const C3DVector&, float)
//   @ 0x00A42140.
// Shape: the normal is copied first (the implicit POD copy moves Z through eax) and the point on the plane is that copy
//   scaled by the distance; the rotated normal is assigned from Normalise's returned reference; every row is the
//   z, y, x accumulation (+ the translation for the point) and the new distance is the z, y, x dot product.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& Normalise();                         // 0x00A14440
};

struct CPlane {
    C3DVector Normal;                               // +0x00
    float     Distance;                             // +0x0C
    bool Initialise(const C3DVector& normal, float distance);   // 0x00A42140
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    C3DVector Rotate(const C3DVector& v) const
    {
        return C3DVector(v.Z * E31 + v.Y * E21 + v.X * E11,
                         v.Z * E32 + v.Y * E22 + v.X * E12,
                         v.Z * E33 + v.Y * E23 + v.X * E13);
    }
    C3DVector Transform(const C3DVector& v) const
    {
        return C3DVector(v.Z * E31 + v.Y * E21 + v.X * E11 + E41,
                         v.Z * E32 + v.Y * E22 + v.X * E12 + E42,
                         v.Z * E33 + v.Y * E23 + v.X * E13 + E43);
    }
    void TransformPlane(CPlane& plane) const;
};

inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + a.Y * b.Y + a.X * b.X; }

void CMatrix3x4::TransformPlane(CPlane& plane) const
{
    C3DVector normal = plane.Normal;
    C3DVector point = normal * plane.Distance;
    C3DVector newNormal = Rotate(normal).Normalise();
    point = Transform(point);
    plane.Initialise(newNormal, DotProduct(point, newNormal));
}
