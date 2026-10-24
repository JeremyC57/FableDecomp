// C2DLineF::GetDist(const C2DVector& point) -- distance from a point to the segment: find the closest point on the line and return the magnitude of the offset to it
// Grade: DIFFER(63v63) -- x87 lifetime only: retail squares Y then X from copies (`fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3) ; faddp`) and stores the root over the differences (`fstp st(2) ; fstp st(0)`); ours squares X in place (`fld st(1) ; fmulp st(2)`) and ends `fxch ; fsqrt ; fstp st(1)` (operand order X*X+Y*Y vs Y*Y+X*X, a named offset local and a user copy ctor all compile the same)
// Leaf: ?GetDist@C2DLineF@@QAEMABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57730, 63 bytes, `ret 4` (PDB ?GetDist@C2DLineF@@QAEMABVC2DVector@@@Z, non-const).
// Callees: C2DLineF::GetPointOnLine(const C2DVector&, C2DVector&, float, C2DVector*, bool*) const @ 0x00A571A0 (its float
//   result is discarded: `fstp st(0)`); sqrt intrinsic.
// Label: the manifest's `C2DLineF_DistanceToPoint` is descriptive.
// Layout: C2DLineF {C2DVector Point1 (+0), Point2 (+8)} as the landed GetDist(const C2DLineF&...) 0x00A57430.
// Shape: `C2DVector closest; GetPointOnLine(point, closest, 0.0f, 0, 0); return (point - closest).GetMagnitude();` --
//   the closest point is an 8-byte frame local, the offset stays on the x87 stack; GetMagnitude is the inline
//   `sqrt(Y * Y + X * X)` (retail squares Y first, both from copies: `fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3)`,
//   the root stored over the differences with `fstp st(2) ; fstp st(0)`), as the landed 0x007A9960 C2DVector::GetLength.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float GetSquaredMagnitude() const { return X * X + Y * Y; }
    float GetMagnitude() const { return (float)sqrt(Y * Y + X * X); }   // retail squares Y first
};

class C2DLineF {
public:
    C2DVector Point1;                                   // +0x00
    C2DVector Point2;                                   // +0x08
    float GetPointOnLine(const C2DVector& p, C2DVector& closest, float radius, C2DVector* projected, bool* before) const;   // 0x00A571A0
    float GetDist(const C2DVector& point);
};

float C2DLineF::GetDist(const C2DVector& point)
{
    C2DVector closest;
    GetPointOnLine(point, closest, 0.0f, 0, 0);
    return (point - closest).GetMagnitude();
}
