// Behaviour test for CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive (0x00BE0EB0): a model of the screen-to-world
// placement -- screen X/Y with linear Z to view space ((X - cx) * Z / halfW, (cy - Y) / halfH * Z), view to world through the
// camera's inverse view matrix (rows E1k*x + E2k*y + E3k*z + E4k), the inverse transform post-scaled by Z / halfW * 100 / scaleX
// and post-translated to the world point -- and the per-pass alpha-ref / z states.
#include <cstdio>
#include <cmath>
struct M { float e[4][3]; };
static void Place(M& t, float X, float Y, float Z, float ohw, float ohh, float cx, float cy, float hvsx, const M& inv)
{
    float vx = (X - cx) * Z * ohw, vy = (cy - Y) * ohh * Z, vz = Z, w[3];
    for (int k = 0; k < 3; ++k) w[k] = inv.e[0][k] * vx + (inv.e[1][k] * vy + inv.e[2][k] * vz) + inv.e[3][k];
    float scale = Z * ohw * 100.0f; scale /= hvsx;
    for (int r = 0; r < 4; ++r) for (int k = 0; k < 3; ++k) t.e[r][k] *= scale;
    for (int k = 0; k < 3; ++k) t.e[3][k] += w[k];
}
static void Pass(int pass, bool writeZ, bool readZ, int& ref, int& zw, int& zt)
{
    if (pass == 6 || pass == 7) { ref = 0; zw = 0; zt = 0; }
    else { if (writeZ) { ref = 0x80; zw = 1; } else { ref = 0; zw = 0; } zt = readZ; }
}
int main()
{
    M inv = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 5, 6, 7 } } };
    M t = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 1, 1 } } };
    Place(t, 420.0f, 200.0f, 2.0f, 1.0f / 320.0f, 1.0f / 240.0f, 320.0f, 240.0f, 1.0f, inv);
    float s = 2.0f / 320.0f * 100.0f;
    bool ok = std::fabs(t.e[0][0] - s) < 1e-5f && std::fabs(t.e[1][1] - s) < 1e-5f
        && std::fabs(t.e[3][0] - (s + 5.0f + 100.0f * 2.0f / 320.0f)) < 1e-4f
        && std::fabs(t.e[3][1] - (s + 6.0f + 40.0f / 240.0f * 2.0f)) < 1e-4f && std::fabs(t.e[3][2] - (s + 7.0f + 2.0f)) < 1e-4f;
    int ref, zw, zt;
    Pass(6, true, true, ref, zw, zt); ok = ok && ref == 0 && zw == 0 && zt == 0;
    Pass(0, true, false, ref, zw, zt); ok = ok && ref == 0x80 && zw == 1 && zt == 0;
    Pass(1, false, true, ref, zw, zt); ok = ok && ref == 0 && zw == 0 && zt == 1;
    printf(ok ? "00be0eb0_TEST PASS\n" : "00be0eb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
