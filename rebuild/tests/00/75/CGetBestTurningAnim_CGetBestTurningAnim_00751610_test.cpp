// 00751610 behaviour test: CGetBestTurningAnim ctor (match.cpp body)
#include <stdio.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
#define NEAR(a, b) (fabs((a) - (b)) < 1e-4)
class C2DVector { public: float X, Y; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise() { float l = (float)sqrt(X * X + Y * Y + Z * Z); if (l > 0) { X /= l; Y /= l; Z /= l; } }
    float Dot(const C3DVector& o) const { return o.X * X + o.Y * Y + o.Z * Z; }
};
class CAnimationEntry;
struct CGetBestTurningAnim {
    float RotationAngle;
    float BestMatchAngle;
    CAnimationEntry* PBestMatchAnim;
    CGetBestTurningAnim(const C2DVector& facing, const C2DVector& target);
};
CGetBestTurningAnim::CGetBestTurningAnim(const C2DVector& facing, const C2DVector& target)
    : BestMatchAngle(0.0f)
{
    C3DVector from(facing.X, facing.Y, 0.0f);
    C3DVector to(target.X, target.Y, 0.0f);
    (void)from.Y;
    to.Z = 0.0f;
    from.Z = 0.0f;
    from.Normalise();
    to.Normalise();
    float cosAngle = from.X * to.X;
    cosAngle += from.Y * to.Y;
    cosAngle += to.Z * from.Z;
    if (cosAngle < -1.0f)
        cosAngle = -1.0f;
    else if (cosAngle > 1.0f)
        cosAngle = 1.0f;
    RotationAngle = (float)acos(cosAngle) * 0.15915494f;
    PBestMatchAnim = 0;
    if (C3DVector(from.Y, -from.X, 0.0f).Dot(to) < 0.0f)
        RotationAngle *= -1.0f;
}
int main()
{
    C2DVector east = { 1, 0 }, north = { 0, 1 }, west = { -1, 0 }, south = { 0, -1 }, ne = { 3, 3 };
    CGetBestTurningAnim a(east, north);   // +90 degrees (counter-clockwise): side test dot = (0,-1).(0,1) = -1 < 0 -> negated
    CHECK(NEAR(a.RotationAngle, -0.25f)); CHECK(a.BestMatchAngle == 0.0f); CHECK(a.PBestMatchAnim == 0);
    CGetBestTurningAnim b(east, south);   // -90 degrees: dot = (0,-1).(0,-1) = 1 -> positive
    CHECK(NEAR(b.RotationAngle, 0.25f));
    CGetBestTurningAnim c(east, east);    // no turn
    CHECK(NEAR(c.RotationAngle, 0.0f));
    CGetBestTurningAnim d(east, west);    // half turn (cos = -1; side dot = 0 -> not negated)
    CHECK(NEAR(d.RotationAngle, 0.5f));
    CGetBestTurningAnim e(east, ne);      // 45 degrees counter-clockwise, lengths normalised
    CHECK(NEAR(e.RotationAngle, -0.125f));
    if (!g_fail) printf("00751610_TEST PASS\n");
    return g_fail;
}