#include <cstdio>
#include <cmath>
// Model of NCameraModes::CProjectileWeapon::GetLookDirectionXYFromParams(phi, heightParam) const @ 0x00878B60:
// the XY camera offset at phi - (initial phi offset / 360 - 0.25) turns, turned into (y, -x, 0) * -1.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CCameraModeDef {
    float ProjectileCameraInitialPhiOffsetForCamera;
    float ProjectileCameraInitialHeightParamForCamera;
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
static const float TWO_PI = 6.2831855f;
static const float ONE_OVER_360 = 1.0f / 360.0f;
class CProjectileWeapon {
public:
    CDefPointer<const CCameraModeDef> PCameraModeDef;
    C3DVector XAxis;
    C3DVector YAxis;
    C3DVector GetCamPosOffsetXY(float fraction) const   // landed 0x00878A80
    {
        double angle = fraction * (double)TWO_PI;
        return XAxis * (float)sin(angle) + YAxis * (float)cos(angle);
    }
    C3DVector GetLookDirectionXYFromParams(float phi, float heightParam) const;
};
C3DVector CProjectileWeapon::GetLookDirectionXYFromParams(float phi, float) const
{
    C3DVector direction = GetCamPosOffsetXY(phi - (PCameraModeDef->ProjectileCameraInitialPhiOffsetForCamera * ONE_OVER_360 - 0.25f));
    direction = C3DVector(direction.Y, -direction.X, 0.0f) * -1.0f;
    return direction;
}
static bool Near(const C3DVector& v, float x, float y)
{
    return fabs(v.X - x) < 1e-5 && fabs(v.Y - y) < 1e-5 && v.Z == 0.0f;
}
int main()
{
    bool ok = true;
    CCameraModeDef def = { 0.0f, 0.5f };
    CProjectileWeapon cam;
    cam.PCameraModeDef.Object = &def;
    cam.XAxis = C3DVector(1.0f, 0.0f, 0.0f);
    cam.YAxis = C3DVector(0.0f, 1.0f, 0.0f);
    if (!Near(cam.GetLookDirectionXYFromParams(0.0f, 0.3f), 0.0f, 1.0f)) ok = false;     // offset (1, 0) -> look +Y
    if (!Near(cam.GetLookDirectionXYFromParams(0.25f, 0.3f), 1.0f, 0.0f)) ok = false;    // offset (0, -1) -> look +X
    def.ProjectileCameraInitialPhiOffsetForCamera = 90.0f;                               // a quarter turn back
    if (!Near(cam.GetLookDirectionXYFromParams(0.0f, 0.3f), -1.0f, 0.0f)) ok = false;    // offset (0, 1) -> look -X
    if (!ok) { std::printf("00878b60_TEST FAIL\n"); return 1; }
    std::printf("00878b60_TEST PASS\n");
    return 0;
}