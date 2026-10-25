// Behaviour test for 0x00878A80 NCameraModes::CProjectileWeapon::GetCamPosOffsetXY(float) const: links against the
// source object (the private member is reached through a friend main).
#include <cstdio>
#include <math.h>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
int main();
namespace NCameraModes {
#pragma pack(push, 1)
class CProjectileWeapon {
public:
    unsigned char _pad[0xa9];
    C3DVector XAxis;
    C3DVector YAxis;
    C3DVector ZAxis;
private:
    C3DVector GetCamPosOffsetXY(float fraction) const;
    friend int ::main();
};
#pragma pack(pop)
}
static bool near1(float a, float b) { return fabs(a - b) < 1e-4; }
int main()
{
    NCameraModes::CProjectileWeapon w;
    if ((char*)&w.XAxis - (char*)&w != 0xa9 || (char*)&w.YAxis - (char*)&w != 0xb5) { std::printf("00878a80_TEST FAIL layout\n"); return 1; }
    w.XAxis = C3DVector(1, 0, 0); w.YAxis = C3DVector(0, 0, 1); w.ZAxis = C3DVector(9, 9, 9);
    C3DVector a = w.GetCamPosOffsetXY(0.0f), b = w.GetCamPosOffsetXY(0.25f), c = w.GetCamPosOffsetXY(0.5f);
    int bad = !near1(a.X, 0) || !near1(a.Z, 1) || !near1(b.X, 1) || !near1(b.Z, 0) || !near1(c.Z, -1) || !near1(c.Y, 0);
    w.XAxis = C3DVector(0, 2, 0); w.YAxis = C3DVector(2, 0, 0);
    C3DVector d = w.GetCamPosOffsetXY(0.125f);
    bad |= !near1(d.X, 1.4142135f) || !near1(d.Y, 1.4142135f) || !near1(d.Z, 0);
    if (bad) { std::printf("00878a80_TEST FAIL\n"); return 1; }
    std::printf("00878a80_TEST PASS\n");
    return 0;
}
