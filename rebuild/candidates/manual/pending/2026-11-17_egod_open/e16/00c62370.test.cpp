// Behaviour test for GetRandomPointInSphere 0x00C62370 (row body copied as plain C++; the random source is scripted).
#include <cstdio>
#include <cmath>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

static const float* g_script; static int g_calls;
float SetLungedThisFrame() { return g_script[g_calls++]; }

static C3DVector GetRandomPointInSphere(float radius)
{
    float phi = SetLungedThisFrame() * 6.28318548f;
    float theta = (float)acos(SetLungedThisFrame() * 2.0f - 1.0f);
    float r = SetLungedThisFrame() * radius;
    C3DVector point((float)sin(theta) * (float)cos(phi) * r, (float)cos(theta) * (float)sin(phi) * r, (float)cos(theta) * r);
    return point;
}

static bool near(float a, double b) { double d = a - b; return d < 1e-4 && d > -1e-4; }

static bool run(float u1, float u2, float u3, float radius)
{
    const float script[3] = { u1, u2, u3 };
    g_script = script; g_calls = 0;
    C3DVector p = GetRandomPointInSphere(radius);
    double phi = u1 * 6.283185307179586, theta = std::acos(u2 * 2.0 - 1.0), r = u3 * (double)radius;
    return g_calls == 3 && near(p.X, std::sin(theta) * std::cos(phi) * r) && near(p.Y, std::cos(theta) * std::sin(phi) * r) &&
           near(p.Z, std::cos(theta) * r);
}

int main()
{
    bool ok = run(0.1f, 0.3f, 0.6f, 5.0f) && run(0.7f, 0.85f, 0.2f, 3.0f) && run(0.35f, 0.05f, 1.0f, 2.5f) &&
              run(0.0f, 0.5f, 0.9f, 1.0f) && run(0.6f, 0.95f, 0.0f, 7.0f);
    // the zero-length case above must give the origin; also a non-trivial case must not be the origin
    const float s[3] = { 0.1f, 0.3f, 0.6f }; g_script = s; g_calls = 0;
    C3DVector q = GetRandomPointInSphere(5.0f);
    ok = ok && !(q.X == 0.0f && q.Y == 0.0f && q.Z == 0.0f);
    std::printf(ok ? "00c62370_TEST PASS\n" : "00c62370_TEST FAIL\n");
    return ok ? 0 : 1;
}
