// Behaviour test for NCameraModes::SetCameraRoll (0x0084CD10): build the up vector perpendicular to the view direction in the
// vertical plane (solving on the larger horizontal axis), normalise it, rotate it about the view direction by roll and set the view.
// The result must be a unit vector perpendicular to forward; with roll 0 it has a positive Z.
#include <stdio.h>
#include <math.h>

struct V { float X, Y, Z; };
static float Dot(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static V Cross(const V& a, const V& b) { V r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
static V g_forward, g_up;
static void SetView(const V& f, const V& u) { g_forward = f; g_up = u; }
static void Normalise(V& v) { float m = (float)sqrt(Dot(v, v)); v.X /= m; v.Y /= m; v.Z /= m; }
static void Rotate(V& v, const V& axisIn, float angle)    // Rodrigues about the normalised axis
{
    V k = axisIn; Normalise(k);
    float c = (float)cos(angle), s = (float)sin(angle), d = Dot(k, v);
    V kv = Cross(k, v);
    V r = { v.X * c + kv.X * s + k.X * d * (1 - c), v.Y * c + kv.Y * s + k.Y * d * (1 - c), v.Z * c + kv.Z * s + k.Z * d * (1 - c) };
    v = r;
}
static void SetCameraRoll(const V& view, float roll)
{
    V forward = view;
    V up = { 0.0f, 0.0f, 1.0f };
    float len2 = (forward.X * forward.X) + (forward.Y * forward.Y);
    if (fabs(forward.X) > fabs(forward.Y)) {
        up.X = -(forward.Z / len2 * forward.X);
        up.Y = up.X / forward.X * forward.Y;
    } else {
        up.Y = -(forward.Z / len2 * forward.Y);
        up.X = up.Y / forward.Y * forward.X;
    }
    Normalise(up);
    Rotate(up, forward, roll);
    SetView(forward, up);
}
int main()
{
    int fails = 0;
    V views[3] = { { 1.0f, 0.2f, -0.5f }, { 0.1f, -2.0f, 0.7f }, { -0.6f, 0.6f, 0.3f } };
    float rolls[3] = { 0.0f, 0.5f, -1.2f };
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            SetCameraRoll(views[i], rolls[j]);
            V f = views[i]; Normalise(f);
            if (fabs(Dot(g_up, f)) > 0.001f || fabs(Dot(g_up, g_up) - 1.0f) > 0.001f) { printf("perp/unit %d %d FAIL\n", i, j); ++fails; }
            if (j == 0 && g_up.Z <= 0.0f) { printf("upright %d FAIL\n", i); ++fails; }
        }
    SetCameraRoll(views[0], 0.0f);
    V a = g_up;
    SetCameraRoll(views[0], 0.5f);
    if (fabs(Dot(a, g_up) - (float)cos(0.5)) > 0.001f) { printf("roll angle FAIL\n"); ++fails; }
    if (fails) { printf("0084cd10_TEST FAIL\n"); return 1; }
    printf("0084cd10_TEST PASS\n");
    return 0;
}