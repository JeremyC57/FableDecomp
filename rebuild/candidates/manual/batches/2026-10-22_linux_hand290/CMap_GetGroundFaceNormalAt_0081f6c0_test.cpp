// Behaviour test for CMap::GetGroundFaceNormalAt (0x0081F6C0): a flat height map gives (0,0,1); the plane h = 2x gives a
// normal along (-2,0,1); on an odd-parity cell the triangle is chosen by dx + dy > 1, on an even one by dy > dx.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static float g_h[4][4];   // [y][x]
static float H(long x, long y) { return g_h[y][x]; }
static V Normal(float px, float py)
{
    long x = (long)px, y = (long)py;
    float h00 = H(x, y), h10 = H(x + 1, y), h01 = H(x, y + 1), h11 = H(x + 1, y + 1);
    float dy = py - (long)py, dx = px - x, a, b;
    if ((x + y) % 2 == 1) {
        if (dy + dx > 1.0f) { a = h11 - h01; b = h11 - h10; } else { a = h10 - h00; b = h01 - h00; }
    } else {
        if (dy > dx) { a = h11 - h01; b = h01 - h00; } else { a = h10 - h00; b = h11 - h10; }
    }
    V n; n.X = -a; n.Y = -b; n.Z = 1.0f;   // (1,0,a) x (0,1,b)
    float r = 1.0f / (float)std::sqrt(n.X * n.X + n.Y * n.Y + n.Z * n.Z);
    n.X *= r; n.Y *= r; n.Z *= r;
    return n;
}
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }

int main()
{
    for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) g_h[y][x] = 3.0f;
    V f = Normal(1.5f, 1.25f);
    bool ok = Near(f.X, 0.0f) && Near(f.Y, 0.0f) && Near(f.Z, 1.0f);
    for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) g_h[y][x] = 2.0f * x;
    V s = Normal(1.2f, 0.7f);
    float k = 1.0f / (float)std::sqrt(5.0f);
    ok = ok && Near(s.X, -2.0f * k) && Near(s.Y, 0.0f) && Near(s.Z, k);
    // a single raised corner (1,1): odd cell (0,1)... pick the triangle that contains it only
    for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) g_h[y][x] = 0.0f;
    g_h[2][2] = 1.0f;   // corner h11 of cell (1,1)
    V lo = Normal(1.6f, 1.2f);   // even cell, dy < dx: a = h10-h00 = 0, b = h11-h10 = 1
    V hi = Normal(1.2f, 1.6f);   // even cell, dy > dx: a = h11-h01 = 1, b = h01-h00 = 0
    ok = ok && Near(lo.X, 0.0f) && lo.Y < -0.7f && hi.X < -0.7f && Near(hi.Y, 0.0f);
    std::printf(ok ? "0081f6c0_TEST PASS\n" : "0081f6c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
