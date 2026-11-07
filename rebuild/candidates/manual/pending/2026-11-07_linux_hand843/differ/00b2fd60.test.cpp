// Behaviour test for CEngineCamera::SetupGamut (0x00B2FD60): identity inverse view, 640x480 window, far plane 100.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V operator-(const V& o) const { return V(X - o.X, Y - o.Y, Z - o.Z); }
    V operator^(const V& o) const { return V(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X); }
    float Dot(const V& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    void AccurateNormalise() { float m = (float)std::sqrt(X * X + Y * Y + Z * Z); X /= m; Y /= m; Z /= m; } };
struct P { V Normal; float Distance; void Initialise(const V& n, float d) { Normal = n; Distance = d; } };
struct Cam {
    float TLX, TLY, FarZ, W, H, OHW, OHH, CX, CY; float M[12]; V Look; P Planes[4];
    void Screen(V& p) const { float z = p.Z; p.X = (p.X - CX) * OHW * z; p.Y = (CY - p.Y) * OHH * z; p.Z = z; }
    void World(V& p) const { float x = p.X, y = p.Y, z = p.Z;
        p.X = M[0] * x + M[3] * y + M[6] * z + M[9]; p.Y = M[1] * x + M[4] * y + M[7] * z + M[10]; p.Z = M[2] * x + M[5] * y + M[8] * z + M[11]; }
    void SetupGamut() {
        V origin(0, 0, 0); V c[4];
        c[0] = V(TLX, TLY, FarZ); c[1] = V(TLX + W, TLY, FarZ); c[2] = V(TLX + W, TLY + H, FarZ); c[3] = V(TLX, TLY + H, FarZ);
        Screen(origin); World(origin);
        for (int i = 0; i < 4; ++i) { Screen(c[i]); World(c[i]); }
        Look = V(0, 0, 1); { float x = Look.X, y = Look.Y, z = Look.Z; Look = V(x * M[0] + (y * M[3] + z * M[6]), x * M[1] + (y * M[4] + z * M[7]), x * M[2] + (y * M[5] + z * M[8])); }
        Look.AccurateNormalise();
        V top = (c[1] - origin) ^ (c[0] - origin), right = (c[2] - origin) ^ (c[1] - origin);
        V bottom = (c[3] - origin) ^ (c[2] - origin), left = (c[0] - origin) ^ (c[3] - origin);
        top.AccurateNormalise(); right.AccurateNormalise(); bottom.AccurateNormalise(); left.AccurateNormalise();
        Planes[2].Initialise(top, top.Dot(origin)); Planes[1].Initialise(right, right.Dot(origin));
        Planes[3].Initialise(bottom, bottom.Dot(origin)); Planes[0].Initialise(left, left.Dot(origin));
    }
};
static bool near1(float a, float b) { return std::fabs(a - b) < 1e-3f; }
int main()
{
    Cam c; c.TLX = 0; c.TLY = 0; c.FarZ = 100; c.W = 640; c.H = 480; c.OHW = 1.0f / 320; c.OHH = 1.0f / 240; c.CX = 320; c.CY = 240;
    for (int i = 0; i < 12; ++i) c.M[i] = 0; c.M[0] = c.M[4] = c.M[8] = 1; c.M[9] = 5; c.M[10] = 0; c.M[11] = 0;
    c.SetupGamut();
    const float h = 0.70710678f;
    if (!near1(c.Look.Z, 1) || !near1(c.Planes[2].Normal.Y, -h) || !near1(c.Planes[2].Normal.Z, h) || !near1(c.Planes[1].Normal.X, -h)
        || !near1(c.Planes[3].Normal.Y, h) || !near1(c.Planes[0].Normal.X, h) || !near1(c.Planes[0].Distance, 5 * h) || !near1(c.Planes[1].Distance, -5 * h)) {
        std::printf("00b2fd60_TEST FAIL\n"); return 1;
    }
    std::printf("00b2fd60_TEST PASS\n");
    return 0;
}
