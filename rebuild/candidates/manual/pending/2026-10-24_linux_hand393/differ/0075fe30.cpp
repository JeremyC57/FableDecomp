// Grade: DIFFER(193v189) -- the Cross().Dot() reduction: ours multiplies Z first with a dead copy of the cross Z kept on the x87 stack (fld mem ; fmul st(1) ... fstp st(0)), retail multiplies X, Z, Y with fxch and no copy; plus B/A pointer registers swapped (ecx/eax) in the first link
// NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation::operator()(const C3DVector& point) const -- true when the XY turn A->B->point circulates the same way as UpDirection
// Leaf: ??RCIsPointFinalLinkInTriangleWithMatchingCirculation@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075FE30, 189 bytes (+int3 pad), `ret 4`.
// Identity: Ego_r.pdb ??RCIsPointFinalLinkInTriangleWithMatchingCirculation@NShape@@QBE_NABVC3DVector@@@Z; its seven
//   callers are the STLport __find_if 0x00763F50 over the shape's points. The catalog's "Polygon_TestPointInside" is an
//   autoname.
// Callees: C3DVector::Normalise() @ 0x00A14440 (twice). Pool: 0.0f @ 0x0122DEDC.
// Shape: both links are flattened to the XY plane through the inline C3DVector(x, y, z) constructor (arguments
//   evaluated right to left: the zero Z stored as an immediate, then Y, then X) and normalised in place; the result is
//   link1.Cross(UpDirection).Dot(link2) > 0 with C3DVector's inline Cross/Dot, every intermediate kept on the x87 stack.
// Layout: Ego_r.pdb CIsPointFinalLinkInTriangleWithMatchingCirculation (0x14): PointA +0, PointB +4, UpDirection +8.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& Normalise();                       // 0x00A14440
    const C3DVector Cross(const C3DVector& v) const
    {
        return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
    }
    const float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

namespace NShape {
class CIsPointFinalLinkInTriangleWithMatchingCirculation {
public:
    C3DVector* PointA;                            // +0x00
    C3DVector* PointB;                            // +0x04
    C3DVector  UpDirection;                       // +0x08
    bool operator()(const C3DVector& point) const;
};

bool CIsPointFinalLinkInTriangleWithMatchingCirculation::operator()(const C3DVector& point) const
{
    C3DVector link1(PointB->X - PointA->X, PointB->Y - PointA->Y, 0.0f);
    link1.Normalise();
    C3DVector link2(point.X - PointB->X, point.Y - PointB->Y, 0.0f);
    link2.Normalise();
    return link1.Cross(UpDirection).Dot(link2) > 0.0f;
}
}
