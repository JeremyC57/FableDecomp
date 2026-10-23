// Behaviour test for 0x005508D0 C3DOrientationInterpolator::C3DOrientationInterpolator(start, end)
#include <cstdio>
static int g_base = 0;
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CRightHandedSet { C3DVector Up, Forward; };
namespace N_0099a2d0 { struct CBase { CBase() { ++g_base; } virtual ~CBase() {} }; } using N_0099a2d0::CBase;
struct C3DOrientationInterpolator : CBase {
    CRightHandedSet StartOrientation, EndOrientation;
    C3DOrientationInterpolator(const CRightHandedSet& s, const CRightHandedSet& e) : CBase(), StartOrientation(s), EndOrientation(e) {}
};
int main()
{
    CRightHandedSet a, b;
    a.Up.X = 1; a.Up.Y = 2; a.Up.Z = 3; a.Forward.X = 4; a.Forward.Y = 5; a.Forward.Z = 6;
    b.Up.X = 7; b.Up.Y = 8; b.Up.Z = 9; b.Forward.X = 10; b.Forward.Y = 11; b.Forward.Z = 12;
    C3DOrientationInterpolator o(a, b);
    bool ok = g_base == 1 && o.StartOrientation.Up.Y == 2 && o.StartOrientation.Forward.Z == 6 && o.EndOrientation.Up.X == 7 && o.EndOrientation.Forward.Z == 12;
    if (!ok) { std::printf("005508d0_TEST FAIL\n"); return 1; }
    std::printf("005508d0_TEST PASS\n");
    return 0;
}