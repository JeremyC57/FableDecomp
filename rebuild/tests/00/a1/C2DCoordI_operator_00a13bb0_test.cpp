#include <cstdio>
struct C2DVector { float X; float Y; };
struct C2DCoordI { long X; long Y; C2DCoordI& operator-=(const C2DVector& v); };
C2DCoordI& C2DCoordI::operator-=(const C2DVector& v)
{
    X = (long)((float)X - v.X);
    Y = (long)((float)Y - v.Y);
    return *this;
}
int main()
{
    C2DCoordI c = { 10, 20 };
    C2DVector v = { 2.5f, -3.5f };
    C2DCoordI& r = (c -= v);
    if (&r != &c || c.X != 7 || c.Y != 23) { std::printf("00a13bb0_TEST FAIL\n"); return 1; }
    std::printf("00a13bb0_TEST PASS\n");
    return 0;
}