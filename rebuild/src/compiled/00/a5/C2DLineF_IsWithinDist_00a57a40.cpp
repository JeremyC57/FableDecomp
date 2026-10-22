// C2DLineF::IsWithinDist(const C2DVector&, float) const -- is the point within `dist` of the segment's bounding circle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A57A40, 144 bytes, no callees (0.5f @ 0x0122F59C, 0.25f @ 0x0123078C).
// Layout from the PDB (C2DLineF: Point1 +0 {X,Y}, Point2 +8 {X,Y}). The midpoint's X is spilled to the
// frame (x87 pressure), the rest stays on the FPU stack: |p - mid|^2 < |P1 - P2|^2 / 4 + dist^2.
// `test ah,5 ; jp` is the ordered `<`; the result is an int (`mov eax,1` / `xor eax,eax`).
// Shape: C2DVector value operators with a user constructor keep every intermediate on the x87 stack
// in source order (mid, dir, rel); the compare is written `bound > rel^2` (left side evaluated first);
// the squared length must be a NAMED float before `* 0.25f`, else VC distributes the scale into
// both squares.
struct C2DVector {
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float GetLengthSquared() const { return X * X + Y * Y; }
};

struct C2DLineF {
    C2DVector Point1;   // +0x00
    C2DVector Point2;   // +0x08
    int IsWithinDist(const C2DVector& pos, float dist) const;
};

int C2DLineF::IsWithinDist(const C2DVector& pos, float dist) const
{
    C2DVector mid = (Point2 + Point1) * 0.5f;
    C2DVector dir = Point1 - Point2;
    C2DVector rel = pos - mid;
    float lengthSquared = dir.GetLengthSquared();
    if (lengthSquared * 0.25f + dist * dist > rel.GetLengthSquared())
        return 1;
    return 0;
}