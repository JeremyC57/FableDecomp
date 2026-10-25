// NBBBMaths::CLinearSpline::GetPointWithParam(float t) const -- clamp t to [0,1], walk the processed points accumulating segment lengths until the segment containing t * path length, and return the point that far along it (the last point when t runs past the end)
// Leaf: ?GetPointWithParam@CLinearSpline@NBBBMaths@@QBE?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57F70, length 380 bytes, `ret 8` (C3DVector through the hidden pointer).
// Callees: NBBBMaths::CLinearSpline::GetLength() const @ 0x00A57EF0 (Ego_r.pdb ?GetLength@CLinearSpline@NBBBMaths@@QBEMXZ;
//   manifest "CPSCSplineBase_GetPathLength"); sqrt intrinsic.
// Name: Ego_r.pdb 0x00BAFFF0 ?GetPointWithParam@CLinearSpline@NBBBMaths@@QBE?AVC3DVector@@M@Z (the class is in namespace
//   NBBBMaths; C3DVector is a class).
//   Float pool: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 1e-5f @ 0x0129BA3C.
// Layout: Ego_r.pdb CLinearSpline with retail 12-byte STLport vectors: ControlPoints +0x00, ProcessedPoints +0x0C (PDB 16-byte
//   containers put it at +0x10). No engine header.
// Shape: the clamp writes back into the parameter, which is then reused as the travelled distance; the segment test is
//   `travelled + length > target - 1e-5f` (the sum stays on the x87 stack and becomes the new distance); the answer is
//   `prev + delta * (target - travelled) * (1.0f / length)` through inline C3DVector operators (the reciprocal is the
//   by-value operand, homed in the dead hidden-pointer slot); C3DVector needs its user copy constructor (pointer-register
//   copies of `prev`); the travelled reset precedes the first copy.
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct C3DVectorArray {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};
namespace NBBBMaths {
class CLinearSpline {
public:
    C3DVectorArray ControlPoints;                  // +0x00
    C3DVectorArray ProcessedPoints;                // +0x0c
    float GetLength() const;                       // 0x00A57EF0
    C3DVector GetPointWithParam(float t) const;
};
}

C3DVector NBBBMaths::CLinearSpline::GetPointWithParam(float t) const
{
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    float target = GetLength() * t;
    const C3DVector* it = ProcessedPoints._M_start;
    t = 0.0f;
    C3DVector prev = *it;
    for (++it; it != ProcessedPoints._M_finish; ++it) {
        C3DVector delta = *it - prev;
        float length = delta.GetLength();
        if (t + length > target - 1e-5f)
            return prev + delta * (target - t) * (1.0f / length);
        t += length;
        prev = *it;
    }
    return prev;
}