// Behaviour test for 0x008EEA30 Mass::GetVel
#include <stdio.h>
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
    C2DVector operator+(const C2DVector& other) const { return C2DVector(X + other.X, Y + other.Y); }
    C2DVector operator-(const C2DVector& other) const { return C2DVector(X - other.X, Y - other.Y); }
    C2DVector operator*(float scale) const { return C2DVector(X * scale, Y * scale); }
};
class Mass {
public:
    C2DVector Pos;
    C2DVector Vel;
    float     IMass;
    float     APos;
    float     AVel;
    float     AIMass;
    Mass() : Pos(1.0f, 2.0f), Vel(0.5f, -0.5f), IMass(1.0f), APos(0.0f), AVel(2.0f), AIMass(1.0f) { }
    virtual ~Mass() { }
    C2DVector GetVel(C2DVector point) const;
};
C2DVector Mass::GetVel(C2DVector point) const
{
    C2DVector r = point - Pos;
    return Vel + C2DVector(r.Y, -r.X) * AVel;
}
int main()
{
    int ok = 1;
    Mass m;
    C2DVector v = m.GetVel(C2DVector(4.0f, 6.0f));
    /* r = (3, 4); perp = (4, -3); * 2 = (8, -6); + (0.5, -0.5) = (8.5, -6.5) */
    if (v.X != 8.5f || v.Y != -6.5f) ok = 0;
    C2DVector w = m.GetVel(m.Pos);
    if (w.X != 0.5f || w.Y != -0.5f) ok = 0;
    printf(ok ? "008eea30_TEST PASS\n" : "008eea30_TEST FAIL\n");
    return ok ? 0 : 1;
}