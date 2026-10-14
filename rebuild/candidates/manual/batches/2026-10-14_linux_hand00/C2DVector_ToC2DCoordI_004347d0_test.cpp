#include <cstdio>
struct C2DCoordI { long X; long Y; C2DCoordI(long x, long y) : X(x), Y(y) {} };
struct C2DVector { float X; float Y; C2DCoordI ToC2DCoordI() const; };
C2DCoordI C2DVector::ToC2DCoordI() const { return C2DCoordI((long)X, (long)Y); }
int main()
{
    C2DVector v = { 3.75f, -2.25f };
    C2DCoordI c = v.ToC2DCoordI();
    if (c.X != 3 || c.Y != -2) { std::printf("004347d0_TEST FAIL\n"); return 1; }
    std::printf("004347d0_TEST PASS\n");
    return 0;
}
