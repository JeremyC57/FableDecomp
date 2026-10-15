// Behaviour test for 0x00A13C60 C3DVector::Rotate
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    void Rotate(const C3DVector& axis, float angle);
};
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4(const C3DVector& a, float angle)      // Rodrigues rotation; the axes are the matrix columns
    {
        float c = std::cos(angle), s = std::sin(angle), t = 1 - c;
        XAxis.X = t * a.X * a.X + c;       XAxis.Y = t * a.X * a.Y + s * a.Z; XAxis.Z = t * a.X * a.Z - s * a.Y;
        YAxis.X = t * a.X * a.Y - s * a.Z; YAxis.Y = t * a.Y * a.Y + c;       YAxis.Z = t * a.Y * a.Z + s * a.X;
        ZAxis.X = t * a.X * a.Z + s * a.Y; ZAxis.Y = t * a.Y * a.Z - s * a.X; ZAxis.Z = t * a.Z * a.Z + c;
        Position.X = Position.Y = Position.Z = 0;
    }
};
void C3DVector::Rotate(const C3DVector& axis, float angle)
{
    CMatrix3x4 rotation(axis, angle);
    C3DVector v(*this);
    X = v.X * rotation.XAxis.X + v.Y * rotation.YAxis.X + v.Z * rotation.ZAxis.X;
    Y = v.X * rotation.XAxis.Y + v.Y * rotation.YAxis.Y + v.Z * rotation.ZAxis.Y;
    Z = v.X * rotation.XAxis.Z + v.Y * rotation.YAxis.Z + v.Z * rotation.ZAxis.Z;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    C3DVector up; up.X = 0; up.Y = 0; up.Z = 1;
    C3DVector v; v.X = 1; v.Y = 0; v.Z = 0;
    v.Rotate(up, 1.5707963f);            // x axis -> y axis about z
    bool ok = Near(v.X, 0) && Near(v.Y, 1) && Near(v.Z, 0);
    v.Rotate(up, 3.14159265f);
    ok = ok && Near(v.X, 0) && Near(v.Y, -1);
    if (!ok) { std::printf("00a13c60_TEST FAIL\n"); return 1; }
    std::printf("00a13c60_TEST PASS\n");
    return 0;
}
