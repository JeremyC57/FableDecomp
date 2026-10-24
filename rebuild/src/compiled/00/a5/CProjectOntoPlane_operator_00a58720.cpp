// CProjectOntoPlane::operator()(const C3DVector& p) const -- project a 3D point onto a plane basis: the dot products of (p - origin) with the two in-plane axes, returned as a C2DVector
// Leaf: ??RCProjectOntoPlane@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A58720, 100 bytes (+28 int3), `ret 8` (thiscall, the C2DVector result
//   through the hidden pointer, then the point). No callees, no callers by address: the out-of-line COMDAT of the
//   functor whose inline copy runs inside the transform at 0x00A5A9D0 (same three borrowed pointers: origin +0,
//   axis X +4, axis Y +8; built by the callers 0x00A58DBC / 0x00A59781 next to IsPointInsideTriangleInThePlane).
// Label: the manifest's `Matrix3x3_TransformPoint` is descriptive; the functor's real name is not in the PDB name set
//   (descriptive class name, shared with the parked 0x00A5A9D0 source).
// Shape: `C3DVector d = p - *Origin; return C2DVector(d * *AxisX, d * *AxisY);` -- the constructor's arguments are
//   evaluated right to left (the AxisY product first), d.x stays on the x87 stack while d.y / d.z are homed in the
//   frame, the two sums are stored through the hidden pointer after an `fxch`.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float operator*(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

// descriptive: the plane-projection functor (three borrowed C3DVector pointers, passed by value to transform)
struct CProjectOntoPlane {
    const C3DVector* Origin;                            // +0x00
    const C3DVector* AxisX;                             // +0x04
    const C3DVector* AxisY;                             // +0x08
    C2DVector operator()(const C3DVector& p) const;
};

C2DVector CProjectOntoPlane::operator()(const C3DVector& p) const
{
    C3DVector d = p - *Origin;
    return C2DVector(d * *AxisX, d * *AxisY);
}