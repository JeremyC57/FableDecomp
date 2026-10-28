extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#include <math.h>
// Behaviour test for 0x0080C140 CThingFilter_IsEvadeExplosion::IsTravellingAway: true when the dot product of the two unit
// directions exceeds cos(0.43633232 rad) (25 degrees).
class C3DVector { public: float X, Y, Z; float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; } };
static bool IsTravellingAway(const C3DVector& a, const C3DVector& b) { return b.Dot(a) > cos(0.43633232f); }
int main()
{
    C3DVector x = { 1, 0, 0 };
    C3DVector at20 = { (float)cos(0.349), (float)sin(0.349), 0 };
    C3DVector at30 = { (float)cos(0.524), (float)sin(0.524), 0 };
    C3DVector back = { -1, 0, 0 };
    CHECK(IsTravellingAway(x, x));
    CHECK(IsTravellingAway(x, at20));
    CHECK(!IsTravellingAway(x, at30));
    CHECK(!IsTravellingAway(x, back));
    if (fails == 0) printf("0080c140_TEST PASS\n"); else printf("0080c140_TEST FAIL\n");
    return fails;
}