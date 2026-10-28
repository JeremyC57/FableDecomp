#include <cstdio>
// Behaviour test for C3DCoordI::operator-= (0x00A13C20): X and Y subtract, Z adds (as retail), truncating toward zero.
class C3DVector { public: float X, Y, Z; };
class C3DCoordI {
public:
    int X, Y, Z;
    const C3DCoordI& operator-=(const C3DVector& v)
    {
        X = (int)((float)X - v.X);
        Y = (int)((float)Y - v.Y);
        Z = (int)((float)Z + v.Z);
        return *this;
    }
};
int main()
{
    C3DCoordI c; c.X = 10; c.Y = -3; c.Z = 7;
    C3DVector v; v.X = 2.5f; v.Y = 1.75f; v.Z = 0.5f;
    const C3DCoordI& r = (c -= v);
    bool ok = &r == &c && c.X == 7 && c.Y == -4 && c.Z == 7;
    if (!ok) { std::printf("00a13c20_TEST FAIL\n"); return 1; }
    std::printf("00a13c20_TEST PASS\n");
    return 0;
}