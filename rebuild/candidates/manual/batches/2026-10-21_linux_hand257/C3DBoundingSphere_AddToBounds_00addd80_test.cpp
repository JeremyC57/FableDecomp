// Behaviour test for C3DBoundingSphere::AddToBounds at 0x00ADDD80: adopt, contain, swallow and span cases.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; };
struct Sphere {
    V3 Center; float Radius;
    void AddToBounds(const V3& p, float r)
    {
        if (Radius == 0.0f) { Center = p; Radius = r; return; }
        if (r == 0.0f) return;
        V3 o; o.X = Center.X - p.X; o.Y = Center.Y - p.Y; o.Z = Center.Z - p.Z;
        float d = (float)std::sqrt(o.X * o.X + o.Y * o.Y + o.Z * o.Z);
        if (Radius < r) { if (r - Radius > d) { Center = p; Radius = r; return; } }
        else if (Radius - r > d) return;
        float k = (Radius - r) / d;
        Center.X = (p.X + Center.X + o.X * k) * 0.5f;
        Center.Y = (p.Y + Center.Y + o.Y * k) * 0.5f;
        Center.Z = (p.Z + Center.Z + o.Z * k) * 0.5f;
        Radius = (d + r + Radius) * 0.5f;
    }
};
static int Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    int ok = 1;
    Sphere s = { { 0, 0, 0 }, 0 };
    V3 p = { 1, 2, 3 };
    s.AddToBounds(p, 2);                                   // empty: adopt
    ok &= Near(s.Center.X, 1) && Near(s.Center.Z, 3) && Near(s.Radius, 2);
    V3 q = { 1, 2, 3.5f };
    s.AddToBounds(q, 0.5f);                                // contained: unchanged
    ok &= Near(s.Radius, 2) && Near(s.Center.Z, 3);
    V3 r = { 1, 2, 4 };
    s.AddToBounds(r, 10);                                  // swallows: adopt
    ok &= Near(s.Center.Z, 4) && Near(s.Radius, 10);
    Sphere t = { { 0, 0, 0 }, 1 };
    V3 u = { 4, 0, 0 };
    t.AddToBounds(u, 1);                                   // span: centre 2, radius 3
    ok &= Near(t.Center.X, 2) && Near(t.Center.Y, 0) && Near(t.Radius, 3);
    std::printf(ok ? "00addd80_TEST PASS\n" : "00addd80_TEST FAIL\n");
    return ok ? 0 : 1;
}
