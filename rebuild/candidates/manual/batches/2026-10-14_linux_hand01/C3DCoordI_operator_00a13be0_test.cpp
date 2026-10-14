#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct C3DCoordI { long X, Y, Z; C3DCoordI& operator+=(const C3DVector& v) { X = (long)((float)X + v.X); Y = (long)((float)Y + v.Y); Z = (long)((float)Z + v.Z); return *this; } };
int main()
{
    C3DCoordI c; c.X = 10; c.Y = -5; c.Z = 3; C3DVector v; v.X = 2.75f; v.Y = -1.5f; v.Z = -4.25f;
    C3DCoordI& r = (c += v);
    if (&r != &c || c.X != 12 || c.Y != -6 || c.Z != -1) { std::printf("00a13be0_TEST FAIL\n"); return 1; }
    std::printf("00a13be0_TEST PASS\n"); return 0;
}
