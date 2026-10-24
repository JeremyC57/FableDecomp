// C2DLineF::GetSquaredDist(const C2DVector& point) -- squared distance from a point to the segment: find the closest point on the line and return the squared magnitude of the offset to it
// Leaf: ?GetSquaredDist@C2DLineF@@QAEMABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57770, 59 bytes, `ret 4` (PDB ?GetSquaredDist@C2DLineF@@QAEMABVC2DVector@@@Z, non-const).
// Callees: C2DLineF::GetPointOnLine(const C2DVector&, C2DVector&, float, C2DVector*, bool*) const @ 0x00A571A0 (its float
//   result is discarded: `fstp st(0)`).
// Label: the manifest's `C2DVector_DistanceToLineSquared` is descriptive.
// Layout: C2DLineF {C2DVector Point1 (+0), Point2 (+8)} as the landed GetDist(const C2DLineF&...) 0x00A57430.
// Shape: `C2DVector closest; GetPointOnLine(point, closest, 0.0f, 0, 0); return (point - closest).GetSquaredMagnitude();` --
//   the closest point is an 8-byte frame local, the offset stays on the x87 stack (inline operator- and GetSquaredMagnitude).
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
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y); }
};

class C2DLineF {
public:
    C2DVector Point1;                                   // +0x00
    C2DVector Point2;                                   // +0x08
    float GetPointOnLine(const C2DVector& p, C2DVector& closest, float radius, C2DVector* projected, bool* before) const;   // 0x00A571A0
    float GetSquaredDist(const C2DVector& point);
};

float C2DLineF::GetSquaredDist(const C2DVector& point)
{
    C2DVector closest;
    GetPointOnLine(point, closest, 0.0f, 0, 0);
    return (point - closest).GetSquaredMagnitude();
}