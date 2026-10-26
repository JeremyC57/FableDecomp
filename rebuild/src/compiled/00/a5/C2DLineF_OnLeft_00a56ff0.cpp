// C2DLineF::OnLeft(const C2DVector& p) const -- is the point on the left of the directed line?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A56FF0, length 64 bytes. No callees (float 0.0f pool @ 0x0122DEDC).
// Layout from the PDB (C2DLineF: Point1 +0 {X,Y}, Point2 +8 {X,Y}). The direction Point2 - Point1
// is formed first (dx then dy), its left normal (-dy, dx) costs one `fchs`, the point is made
// relative to Point1 and the dot product is compared with 0.0f (`test ah,41h ; jne` = ordered `> 0`).
// The x87 stack order (dx below -dy) is what makes VC multiply the X pair first.
class C2DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector(float x, float y) : X(x), Y(y) {}
    float Dot(const C2DVector& o) const { return X * o.X + Y * o.Y; }
};

class C2DLineF { public:
    C2DVector Point1;   // +0x00
    C2DVector Point2;   // +0x08
    bool OnLeft(const C2DVector& pos) const;
};

bool C2DLineF::OnLeft(const C2DVector& pos) const
{
    C2DVector dir(Point2.X - Point1.X, Point2.Y - Point1.Y);
    C2DVector normal(-dir.Y, dir.X);
    C2DVector rel(pos.X - Point1.X, pos.Y - Point1.Y);
    return rel.Dot(normal) > 0.0f ? true : false;
}