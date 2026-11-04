// Behaviour test for 0x00F35C60 NFractal::CNoise::PerlinNoise(const C2DVector&) const: the noise2 body over a fixed permutation / gradient table; checks zero at lattice points, continuity across a cell edge and the [-1, 1] bound.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

static int P[512]; static float G[512][2];
static float Noise(float x, float y)
{
    float t = x + 4096.0f; int bx0 = ((int)t) & 255, bx1 = (bx0 + 1) & 255; float rx0 = t - (int)t, rx1 = rx0 - 1.0f;
    t = y + 4096.0f; int by0 = ((int)t) & 255, by1 = (by0 + 1) & 255; float ry0 = t - (int)t, ry1 = ry0 - 1.0f;
    int i = P[bx0], j = P[bx1]; int b00 = P[i + by0], b10 = P[j + by0], b01 = P[i + by1], b11 = P[j + by1];
    float sx = rx0 * rx0 * (3.0f - 2.0f * rx0), sy = ry0 * ry0 * (3.0f - 2.0f * ry0);
    float u = rx0 * G[b00][0] + ry0 * G[b00][1], v = rx1 * G[b10][0] + ry0 * G[b10][1]; float a = u + sx * (v - u);
    u = rx0 * G[b01][0] + ry1 * G[b01][1]; v = rx1 * G[b11][0] + ry1 * G[b11][1]; float b = u + sx * (v - u);
    return a + sy * (b - a);
}
int main()
{
    for (int i = 0; i < 256; ++i) { P[i] = P[i + 256] = (i * 37 + 11) & 255; float a = i * 0.0245f; G[i][0] = G[i + 256][0] = std::cos(a); G[i][1] = G[i + 256][1] = std::sin(a); }
    int ok = 1;
    ok &= std::fabs(Noise(3.0f, 5.0f)) < 1e-4f;
    ok &= std::fabs(Noise(2.9999f, 4.5f) - Noise(3.0001f, 4.5f)) < 1e-2f;
    for (float x = 0; x < 4; x += 0.37f) for (float y = 0; y < 4; y += 0.41f) ok &= std::fabs(Noise(x, y)) <= 1.0f;
    std::printf(ok ? "00F35C60_TEST PASS\n" : "00F35C60_TEST FAIL\n"); return ok ? 0 : 1;
}
