// Behaviour test for 0x005508D0 C3DOrientationInterpolator::C3DOrientationInterpolator(start, end)
#include <cstdio>
static int g_base = 0;
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CBaseClass {
public:  CBaseClass() { ++g_base; } virtual ~CBaseClass() {} };
class C3DOrientationInterpolator : public CBaseClass { public:
    CRightHandedSet StartOrientation, EndOrientation;
    C3DOrientationInterpolator(const CRightHandedSet& s, const CRightHandedSet& e) : CBaseClass(), StartOrientation(s), EndOrientation(e) {}
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