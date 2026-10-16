// Grade: DIFFER(210v210) -- frame 0x14 v 0x20 and slot placement of the y2/x4 copies (retail homes Point2 at +0x18 and other.Point2 at +0x10 as 8-byte pairs); the x2*y1 / x4*y3 operand loads come out swapped. probe C2DVector p2/p4 locals got the retail frame but copied all four words; next: p2/p4 with a __forceinline memberwise user copy ctor
// C2DLineF::GetInfiniteLineLineIntersection(const C2DLineF&, C2DVector&) const -- intersect the two infinite lines; false when parallel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57960, length 224 bytes (`ret 8`).
// Callees: none (float pool: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8).
// Shape: the determinant form: c1 = x1*y2 - y1*x2, c2 = x3*y4 - y3*x4, det = (x1-x2)(y3-y4) - (y1-y2)(x3-x4);
// a zero determinant returns false, else the point is ((x3-x4)c1 - (x1-x2)c2, (y3-y4)c1 - (y1-y2)c2) * (1/det).
// Layout: PDB C2DLineF (16 bytes): Point1 +0, Point2 +8 (C2DVector X, Y).
// Leaf: GetInfiniteLineLineIntersection
struct C2DVector { float X, Y; };

struct C2DLineF {
    C2DVector Point1;                      // +0
    C2DVector Point2;                      // +8
    bool GetInfiniteLineLineIntersection(const C2DLineF& other, C2DVector& out) const;
};

bool C2DLineF::GetInfiniteLineLineIntersection(const C2DLineF& other, C2DVector& out) const
{
    float x1 = Point1.X, y1 = Point1.Y, x2 = Point2.X, y2 = Point2.Y;
    float x3 = other.Point1.X, y3 = other.Point1.Y, x4 = other.Point2.X, y4 = other.Point2.Y;
    float c1 = x1 * y2 - y1 * x2;
    float c2 = x3 * y4 - y3 * x4;
    float dy2 = y3 - y4;
    float dx1 = x1 - x2;
    float dx2 = x3 - x4;
    float dy1 = y1 - y2;
    float det = dx1 * dy2 - dy1 * dx2;
    if (det == 0.0f)
        return false;
    float inv = 1.0f / det;
    out.X = (dx2 * c1 - dx1 * c2) * inv;
    out.Y = (dy2 * c1 - dy1 * c2) * inv;
    return true;
}
