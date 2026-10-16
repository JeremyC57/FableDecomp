// behaviour test for NCameraModes::CProjectileWeapon::GetRotatedPosition (0x00878A80)
#include <cstdio>
#include <math.h>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); } };
static const float TWO_PI = 6.2831855f;
struct CProjectileWeapon { C3DVector XAxis, YAxis;
    C3DVector GetRotatedPosition(float fraction) const
    { double angle = fraction * (double)TWO_PI; return XAxis * (float)sin(angle) + YAxis * (float)cos(angle); } };
static bool near1(float a, float b) { return fabs(a - b) < 1e-4; }
int main()
{
    CProjectileWeapon w; w.XAxis = C3DVector(1, 0, 0); w.YAxis = C3DVector(0, 0, 1);
    C3DVector a = w.GetRotatedPosition(0.0f), b = w.GetRotatedPosition(0.25f), c = w.GetRotatedPosition(0.5f);
    int bad = !near1(a.X, 0) || !near1(a.Z, 1) || !near1(b.X, 1) || !near1(b.Z, 0) || !near1(c.Z, -1) || !near1(c.Y, 0);
    if (bad) { std::printf("00878a80_TEST FAIL\n"); return 1; }
    std::printf("00878a80_TEST PASS\n");
    return 0;
}