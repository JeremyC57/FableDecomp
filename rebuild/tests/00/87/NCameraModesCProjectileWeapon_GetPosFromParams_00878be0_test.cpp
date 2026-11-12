// Behaviour test for NCameraModes::CProjectileWeapon::GetPosFromParams (0x00878BE0): position = thing position / 1 +
// (XAxis*sin(2*pi*phi) + YAxis*cos(2*pi*phi)) * interp(1, 1, (h' - 0.5) / 2) + ZAxis * h', with h' = h*2 + 0.5.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V3 operator+(const V3& o) const { return V3(X + o.X, Y + o.Y, Z + o.Z); }
    V3 operator*(float s) const { return V3(X * s, Y * s, Z * s); }
    V3 operator/(float v) const { float r = 1.0f / v; return V3(X * r, Y * r, Z * r); } };
static float Interp(float from, float to, float t) { return from + (to - from) * t; }
struct Cam { V3 XAxis, YAxis, ZAxis, ThingPos;
    V3 GetCamPosOffsetXY(float fraction) const { double a = fraction * (double)6.2831855f; return XAxis * (float)sin(a) + YAxis * (float)cos(a); }
    V3 GetPosFromParams(float phi, float height_param) const {
        height_param = height_param * 2.0f + 0.5f;
        float t = (height_param - 0.5f) / 2.0f;
        float scale = Interp(1.0f, 1.0f, t);
        V3 cam_offset = GetCamPosOffsetXY(phi);
        V3 offset = cam_offset * scale;
        V3 pos = ThingPos;
        return pos / 1.0f + offset + ZAxis * height_param; } };
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    Cam c; c.XAxis = V3(1, 0, 0); c.YAxis = V3(0, 1, 0); c.ZAxis = V3(0, 0, 1); c.ThingPos = V3(10, 20, 30);
    V3 a = c.GetPosFromParams(0.0f, 0.25f);          // cos term only: +Y by 1, height 0.25*2+0.5 = 1
    V3 b = c.GetPosFromParams(0.25f, 1.0f);          // sin term only: +X by 1, height 2.5
    V3 d = c.GetPosFromParams(0.5f, 0.0f);           // -Y by 1, height 0.5
    bool ok = Near(a.X, 10) && Near(a.Y, 21) && Near(a.Z, 31)
           && Near(b.X, 11) && Near(b.Y, 20) && Near(b.Z, 32.5f)
           && Near(d.X, 10) && Near(d.Y, 19) && Near(d.Z, 30.5f);
    if (!ok) { std::printf("00878be0_TEST FAIL\n"); return 1; }
    std::printf("00878be0_TEST PASS\n");
    return 0;
}