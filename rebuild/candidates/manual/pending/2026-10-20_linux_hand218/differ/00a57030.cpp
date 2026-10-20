// C2DLineF::Intersects2D(const C2DLineF& other) const -- do two 2D segments touch or cross?
// Grade: DIFFER(ours v retail) -- this/other registers swapped (edx=this, esi=other vs retail esi/edx) and first OnLeft pair block layout
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A57030, 272 bytes. Callee: C2DLineF::OnLeft 0x00A56FF0
// (its real landed body sits above under auto_inline(off): retail keeps ecx/edx live across the
// calls, so the compiler knows the callee's register use). Tolerance: the 1e-4f global @ 0x0129BA3C.
// Layout from the PDB (C2DLineF: Point1 +0 {X,Y}, Point2 +8 {X,Y}).
// Shape: four shared-endpoint tests (`fabs(a-b) <= eps` on X then Y = `test ah,41h ; jp`) return
// true; otherwise the segments cross iff each straddles the other (OnLeft results differ).
#include <math.h>

struct C2DVector {
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector(float x, float y) : X(x), Y(y) {}
    float Dot(const C2DVector& o) const { return X * o.X + Y * o.Y; }
    __forceinline bool IsSame(const C2DVector& o) const;
};

extern float g_epsilon_0129ba3c;   // 0x0129BA3C (1e-4f)

__forceinline bool C2DVector::IsSame(const C2DVector& b) const
{
    return (float)fabs(X - b.X) <= g_epsilon_0129ba3c && (float)fabs(Y - b.Y) <= g_epsilon_0129ba3c;
}

struct C2DLineF {
    C2DVector Point1;   // +0x00
    C2DVector Point2;   // +0x08
    bool OnLeft(const C2DVector& p) const;            // 0x00A56FF0
    bool Intersects2D(const C2DLineF& other) const;
};

#pragma auto_inline(off)
bool C2DLineF::OnLeft(const C2DVector& p) const
{
    C2DVector dir(Point2.X - Point1.X, Point2.Y - Point1.Y);
    C2DVector normal(-dir.Y, dir.X);
    C2DVector rel(p.X - Point1.X, p.Y - Point1.Y);
    return rel.Dot(normal) > 0.0f ? true : false;
}
#pragma auto_inline(on)

bool C2DLineF::Intersects2D(const C2DLineF& other) const
{
    if (Point1.IsSame(other.Point1) || Point1.IsSame(other.Point2) ||
        Point2.IsSame(other.Point1) || Point2.IsSame(other.Point2))
        return true;
    bool b = OnLeft(other.Point2);
    bool a = OnLeft(other.Point1);
    if (a) {
        if (b)
            return false;
    } else if (!b)
        return false;
    {
        bool d = other.OnLeft(Point2);
        bool c = other.OnLeft(Point1);
        if ((c && !d) || (!c && d))
            return true;
    }
    return false;
}
