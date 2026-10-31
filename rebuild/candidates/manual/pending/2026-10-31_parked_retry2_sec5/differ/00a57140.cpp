// C2DLineF::GetPointOnInfiniteLine(const C2DVector& point) const -- project a point onto the infinite line through Point1 and Point2: Point1 + d * ((point - Point1).d / d.d) with d = Point2 - Point1
// Leaf: ?GetPointOnInfiniteLine@C2DLineF@@QBE?AVC2DVector@@ABV2@@Z
// Grade: DIFFER(80v90) -- stepwise Y-first sums inside the inline Dot and GetSquaredMagnitude (float d = Y * o.Y; d += X
//   * o.X): the dot and squared-length products now form in retail's order (25 -> 13 diff lines); residue: retail spills
//   (point.X - Point1.X) and d.X * t to [esp] (fstp/fld) where ours keeps them on the x87 stack (fxch / fmulp), so the
//   d*t cleanup fstp st(2)/fstp st(0) is missing; named offset / scaled temporaries give the same
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A57140, 90 bytes, `ret 8` (hidden C2DVector result, point). Name: Ego_r.pdb
//   ?GetPointOnInfiniteLine@C2DLineF@@QBE?AVC2DVector@@ABV2@@Z; the catalog label agrees.
// Layout: Ego_r.pdb C2DLineF {C2DVector Point1 +0, Point2 +8}, as the landed C2DLineF rows (GetSquaredDist 0x00A57770).
// Shape: the direction is a named local kept on the x87 stack; the projection factor is a dot product over its squared
//   length (Y products first, the inline Dot); the result is built through the C2DVector value constructor.
class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float Dot(const C2DVector& o) const { float d = Y * o.Y; d += X * o.X; return d; }
    float GetSquaredMagnitude() const { float d = Y * Y; d += X * X; return d; }
};

class C2DLineF {
public:
    C2DVector Point1;                                   // +0x00
    C2DVector Point2;                                   // +0x08
    C2DVector GetPointOnInfiniteLine(const C2DVector& point) const;
};

C2DVector C2DLineF::GetPointOnInfiniteLine(const C2DVector& point) const
{
    C2DVector d = Point2 - Point1;
    float t = (point - Point1).Dot(d) / d.GetSquaredMagnitude();
    return Point1 + d * t;
}
