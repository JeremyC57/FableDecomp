// Behaviour test for 0x00A22590 CQuaternion::SetSlerp(a, b, t): the slerp weights from the arc-cos / cosine tables
// (end points reproduce a and b, the half-way point of a 90 degree turn is the 45 degree turn, a negated b gives the same
// rotation through the sign fold, near-parallel inputs take the plain lerp weights).
#include <stdio.h>
#include <math.h>

static float g_ArcCosTable[1026];
static float g_CosTable[1025];

static long FloatToLong(float value) { return (long)floor(value + 0.5); }
static float GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = FloatToLong(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
static float GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

class CQuaternion { public:
    float X, Y, Z, W;
    void SetSlerp(const CQuaternion& a, const CQuaternion& b, float t)
    {
        float cosom = a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
        float sign = 1.0f;
        if (cosom < 0.0f) {
            sign = -1.0f;
            cosom = -cosom;
        }
        float scale0;
        float scale1;
        if (1.0f - cosom > 0.05f) {
            float omega = GFFastArcCos(cosom);
            float sinom = (float)sqrt(1.0f - cosom * cosom);
            scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
            scale1 = GFFastSin(omega * t) * (1.0f / sinom);
        }
        else {
            scale0 = 1.0f - t;
            scale1 = t;
        }
        scale1 *= sign;
        X = scale0 * a.X + scale1 * b.X;
        Y = scale0 * a.Y + scale1 * b.Y;
        Z = scale0 * a.Z + scale1 * b.Z;
        W = scale0 * a.W + scale1 * b.W;
    }
};

static int fail(int line) { printf("00a22590_TEST FAIL (line %d)\n", line); return 1; }
static bool Near(float a, float b, float eps) { return fabs(a - b) < eps; }
static bool NearQ(const CQuaternion& q, float x, float y, float z, float w, float eps)
{
    return Near(q.X, x, eps) && Near(q.Y, y, eps) && Near(q.Z, z, eps) && Near(q.W, w, eps);
}

int main()
{
    const double pi = 3.14159265358979;
    for (int i = 0; i < 1026; ++i) {
        double c = i / 512.5 - 1.0;
        if (c > 1.0) c = 1.0;
        g_ArcCosTable[i] = (float)acos(c);
    }
    for (int i = 0; i < 1025; ++i)
        g_CosTable[i] = (float)cos(2.0 * pi * i / 1024.0);

    CQuaternion identity = { 0.0f, 0.0f, 0.0f, 1.0f };
    float h = (float)sqrt(0.5);
    CQuaternion turn90 = { 0.0f, 0.0f, h, h };   // 90 degrees about Z
    CQuaternion q;
    q.SetSlerp(identity, turn90, 0.0f);
    if (!NearQ(q, 0.0f, 0.0f, 0.0f, 1.0f, 0.01f)) return fail(__LINE__);
    q.SetSlerp(identity, turn90, 1.0f);
    if (!NearQ(q, 0.0f, 0.0f, h, h, 0.01f)) return fail(__LINE__);
    q.SetSlerp(identity, turn90, 0.5f);   // 45 degrees about Z: (0, 0, sin 22.5, cos 22.5)
    if (!NearQ(q, 0.0f, 0.0f, (float)sin(pi / 8), (float)cos(pi / 8), 0.01f)) return fail(__LINE__);
    // b on the other hemisphere: the sign fold gives the same rotation
    CQuaternion neg90 = { 0.0f, 0.0f, -h, -h };
    q.SetSlerp(identity, neg90, 0.5f);
    if (!NearQ(q, 0.0f, 0.0f, (float)sin(pi / 8), (float)cos(pi / 8), 0.01f)) return fail(__LINE__);
    // near-parallel: plain lerp weights (1 - t, t)
    CQuaternion a = { 0.0f, 0.0f, 0.0f, 1.0f };
    CQuaternion b = { 0.1f, 0.0f, 0.0f, 0.995f };
    q.SetSlerp(a, b, 0.25f);
    if (!NearQ(q, 0.025f, 0.0f, 0.0f, 0.75f + 0.25f * 0.995f, 1e-5f)) return fail(__LINE__);
    printf("00a22590_TEST PASS\n");
    return 0;
}