// Behaviour test for 0x00AA3560 CQuaternion::SetSlerpFromIdentity(const CQuaternion&, float)
#include <cstdio>
#include <cmath>

static const float kPi = 3.14159265358979f;
static float g_ArcCosTable[1026];       // acos(k*2/1025 - 1)
static float g_CosTable[1025];          // cos(2*pi*k/1024)
static void InitTables()
{
    for (int k = 0; k < 1026; ++k) { float c = k * 2.0f / 1025.0f - 1.0f; if (c > 1.0f) c = 1.0f; g_ArcCosTable[k] = (float)std::acos(c); }
    for (int k = 0; k < 1025; ++k) g_CosTable[k] = (float)std::cos(2.0 * kPi * k / 1024.0);
}
static long FloatToLong(float v) { return (long)std::floor(v + 0.5f); }   // the engine's round-to-nearest helper
static float GFFastArcCos(float c)
{
    c = (c + 1.0f) * 512.5f;
    long i = FloatToLong(c - 0.5f);
    float frac = c - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
static float GFFastSin(float a)
{
    a = a * 1024.0f;
    long i = FloatToLong(a - 0.5f);
    float frac = a - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
struct Q { float X, Y, Z, W; };
static void Weights(float cosom, float t, float& s0, float& s1)   // shared slerp-weight body of the three rows
{
    float sign = 1.0f;
    if (cosom < 0.0f) { sign = -1.0f; cosom = -cosom; }
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)std::sqrt(1.0f - cosom * cosom);
        s0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        s1 = GFFastSin(omega * t) * (1.0f / sinom);
    }
    else { s0 = 1.0f - t; s1 = t; }
    s1 = sign * s1;
}
static void Exact(float cosom, float t, float& s0, float& s1)      // textbook slerp weights for comparison
{
    float sign = cosom < 0 ? -1.0f : 1.0f; cosom = std::fabs(cosom);
    if (1.0f - cosom > 0.05f) { float om = (float)std::acos(cosom); s0 = (float)(std::sin((1 - t) * om) / std::sin(om)); s1 = (float)(std::sin(t * om) / std::sin(om)); }
    else { s0 = 1 - t; s1 = t; }
    s1 *= sign;
}
static int Near(float a, float b) { return std::fabs(a - b) < 0.01f; }
static Q Mul(const Q& a, const Q& b)                                // CQuaternion::operator* 0x00A88B60
{
    Q r;
    r.X = a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y;
    r.Y = a.W * b.Y - a.X * b.Z + a.Y * b.W + a.Z * b.X;
    r.Z = a.W * b.Z + a.X * b.Y - a.Y * b.X + a.Z * b.W;
    r.W = a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z;
    return r;
}

static void SetSlerpFromIdentity(Q& self, const Q& q, float t)
{
    float s0, s1;
    Weights(q.W, t, s0, s1);
    self.X = s1 * q.X; self.Y = s1 * q.Y; self.Z = s1 * q.Z; self.W = s1 * q.W + s0;
}
int main()
{
    InitTables();
    int ok = 1;
    const float h = 0.70710678f;
    Q rz90 = { 0, 0, h, h };
    Q r;
    SetSlerpFromIdentity(r, rz90, 0.5f);                              // half of 90 degrees about z: 45 degrees
    ok &= Near(r.Z, (float)std::sin(kPi / 8)) && Near(r.W, (float)std::cos(kPi / 8)) && r.X == 0.0f;
    SetSlerpFromIdentity(r, rz90, 0.0f);
    ok &= Near(r.W, 1.0f) && Near(r.Z, 0.0f);
    SetSlerpFromIdentity(r, rz90, 1.0f);
    ok &= Near(r.W, h) && Near(r.Z, h);
    Q negq = { 0, 0, -h, -h };                                        // the same rotation, other hemisphere
    SetSlerpFromIdentity(r, negq, 0.5f);
    ok &= Near(r.Z, (float)std::sin(kPi / 8)) && Near(r.W, (float)std::cos(kPi / 8));
    if (ok) std::printf("00aa3560_TEST PASS\n"); else std::printf("00aa3560_TEST FAIL\n");
    return ok ? 0 : 1;
}