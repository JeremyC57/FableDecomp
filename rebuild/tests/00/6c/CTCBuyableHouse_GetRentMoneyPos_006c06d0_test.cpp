// Behaviour test for CTCBuyableHouse::GetRentMoneyPos (0x006C06D0): a copy of the row's logic on stand-alone types.
#include <stdio.h>
#include <math.h>
struct V3 { float X, Y, Z; };
struct V2 { float X, Y;
    void Rotate(float revolutions) { float a = revolutions * 6.2831853f; float c = cosf(a), s = sinf(a); float x = X * c - Y * s; Y = X * s + Y * c; X = x; } };
struct RH { V3 Up, Forward; };
static bool g_hasPost; static V3 g_postPos; static RH g_postRH;
static bool GetSignPostPos(bool main, V3& pos, RH* rh) { if (!main || !g_hasPost) return false; pos = g_postPos; *rh = g_postRH; return true; }
static bool GetRentMoneyPos(V3& ret, long bag, long bags)
{
    V3 pos; RH rhs;
    if (GetSignPostPos(true, pos, &rhs)) {
        V3 forward = rhs.Forward;
        float squared = forward.Z * forward.Z; squared += forward.X * forward.X; squared += forward.Y * forward.Y;
        float r = 1.0f / sqrtf(squared);
        forward.X *= r; forward.Y *= r; forward.Z *= r;
        pos.X += forward.X; pos.Y += forward.Y; pos.Z += forward.Z;
        V2 offset = { rhs.Forward.X * 0.7f, rhs.Forward.Y * 0.7f };
        offset.Rotate((float)bag / bags);
        pos.X += offset.X; pos.Y += offset.Y;
        ret = pos;
        return true;
    }
    return false;
}
static bool near(float a, float b) { return fabs(a - b) < 1e-3f; }
int main()
{
    int fails = 0;
    V3 out = { -9.0f, -9.0f, -9.0f };
    g_hasPost = false;
    if (GetRentMoneyPos(out, 0, 4) || out.X != -9.0f) ++fails;            // no sign post: false, ret untouched
    g_hasPost = true;
    g_postPos.X = 10.0f; g_postPos.Y = 20.0f; g_postPos.Z = 3.0f;
    g_postRH.Forward.X = 2.0f; g_postRH.Forward.Y = 0.0f; g_postRH.Forward.Z = 0.0f;   // unnormalised forward (+X, length 2)
    if (!GetRentMoneyPos(out, 0, 4) || !near(out.X, 11.0f + 1.4f) || !near(out.Y, 20.0f) || !near(out.Z, 3.0f)) ++fails;
    // a quarter turn: the 1.4 offset swings to +Y
    if (!GetRentMoneyPos(out, 1, 4) || !near(out.X, 11.0f) || !near(out.Y, 21.4f) || !near(out.Z, 3.0f)) ++fails;
    // forward with Z: normalised step uses all three axes, the circle only X / Y of the raw forward
    g_postRH.Forward.X = 0.0f; g_postRH.Forward.Y = 3.0f; g_postRH.Forward.Z = 4.0f;
    if (!GetRentMoneyPos(out, 2, 4) || !near(out.X, 10.0f) || !near(out.Y, 20.0f + 0.6f - 2.1f) || !near(out.Z, 3.8f)) ++fails;
    printf(fails ? "006c06d0_TEST FAIL\n" : "006c06d0_TEST PASS\n");
    return fails ? 1 : 0;
}