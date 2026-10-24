// NBBBMaths::CLinearSpline::GetLength() const -- total length of the processed polyline: the sum of the distances between consecutive processed points
// Leaf: ?GetLength@CLinearSpline@NBBBMaths@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57EF0, 128 bytes, plain `ret`, float in st(0)
//   (PDB ?GetLength@CLinearSpline@NBBBMaths@@QBEMXZ). Callees: none (fsqrt inline). Float pool: 0.0f @ 0x0122DEDC.
//   Caller: CLinearSpline::GetPointWithParam @ 0x00A57F70 (landed, which calls this "GetPathLength").
// Label: the manifest's `CPSCSplineBase_GetPathLength` is descriptive (the particle spline component is a different class).
// Layout: Ego_r.pdb CLinearSpline with retail 12-byte STLport vectors (the landed 0x00A57F70 model): ControlPoints +0x00,
//   ProcessedPoints +0x0C. No engine header.
// Shape: `prev = *first; for (++it; it != last; ++it) { length += (*it - prev).GetLength(); prev = *it; }` -- the
//   difference is an unnamed temporary whose three components stay on the x87 stack, GetLength is the inline
//   `(float)sqrt(GetLengthSquared())` (squares from copies, Z first; the root stored over the differences:
//   `fstp st(3) ; fstp st(0) ; fstp st(0)`), prev is a 12-byte frame local built by the user copy constructor (memberwise through edx) and refreshed by the
//   implicit assignment (a block copy through a second pointer register) inside the loop.
#include <math.h>
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float GetLength() const { return (float)sqrt(GetLengthSquared()); }
};

namespace NBBBMaths {

struct C3DVectorArray {                                 // STLport vector<C3DVector> (12 bytes in retail)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};

class CLinearSpline {
public:
    C3DVectorArray ControlPoints;                       // +0x00
    C3DVectorArray ProcessedPoints;                     // +0x0c
    float GetLength() const;
};

float CLinearSpline::GetLength() const
{
    float length = 0.0f;
    const C3DVector* it = ProcessedPoints._M_start;
    C3DVector prev = *it;
    for (++it; it != ProcessedPoints._M_finish; ++it) {
        length += (*it - prev).GetLength();
        prev = *it;
    }
    return length;
}

}   // namespace NBBBMaths