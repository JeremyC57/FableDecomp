// C2DLineF::GetInfiniteLineLineIntersection(const C2DLineF& other, C2DVector& intersection) const -- intersect the two infinite lines; false when they are parallel
// Leaf: ?GetInfiniteLineLineIntersection@C2DLineF@@QBE_NABV1@AAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A57960, length 210 bytes (`ret 8`). Callees: none
// (float pool: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8).
// Layout: PDB C2DLineF (class, 16 bytes): Point1 +0, Point2 +8; C2DVector (class, 8 bytes): X +0, Y +4.
// Math: the determinant form: c1 = x1*y2 - x2*y1, c2 = x3*y4 - x4*y3, det = (x1-x2)(y3-y4) - (y1-y2)(x3-x4);
// a zero determinant returns false, else the point is ((x3-x4)c1 - (x1-x2)c2, (y3-y4)c1 - (y1-y2)c2) * (1/det).
// Shape: the four end points are copied into C2DVector locals with a memberwise user copy constructor. VC7.1 splits
// each copy into its two float scalars; the ones that stay on the x87 stack vanish, the two that spill (p2.Y, p4.X)
// keep their homes inside 8-byte struct slots, which is retail's 0x20 frame with the gaps at +0x14/+0x18
// (y2 @ +0x1C, x4 @ +0x10). Plain float locals give a packed 0x14 frame; an implicit copy moves all four words.
// The cross terms are written `x2 * y1` / `x4 * y3` (right operand loaded first gives retail's `fld y1 ; fmul x2`).
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

class C2DLineF {
public:
    C2DVector Point1;                      // +0
    C2DVector Point2;                      // +8
    bool GetInfiniteLineLineIntersection(const C2DLineF& other, C2DVector& intersection) const;
};

bool C2DLineF::GetInfiniteLineLineIntersection(const C2DLineF& other, C2DVector& intersection) const
{
    C2DVector p1 = Point1, p2 = Point2, p3 = other.Point1, p4 = other.Point2;
    float c1 = p1.X * p2.Y - p2.X * p1.Y;
    float c2 = p3.X * p4.Y - p4.X * p3.Y;
    float dy2 = p3.Y - p4.Y;
    float dx1 = p1.X - p2.X;
    float dx2 = p3.X - p4.X;
    float dy1 = p1.Y - p2.Y;
    float det = dx1 * dy2 - dy1 * dx2;
    if (det == 0.0f)
        return false;
    float inv = 1.0f / det;
    intersection.X = (dx2 * c1 - dx1 * c2) * inv;
    intersection.Y = (dy2 * c1 - dy1 * c2) * inv;
    return true;
}