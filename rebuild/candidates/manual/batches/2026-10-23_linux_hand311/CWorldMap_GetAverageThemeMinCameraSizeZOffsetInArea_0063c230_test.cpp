// Behaviour test for CWorldMap::GetAverageThemeMinCameraSizeZOffsetInArea (0x0063C230): box edge samples at the corners and each whole-block step, then every inner block.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; };
static int g_edgeCalls, g_blockCalls;
static float EdgeAt(const V3& p) { ++g_edgeCalls; return p.X * 0.5f - p.Y; }
static float BlockAt(int x, int y) { ++g_blockCalls; if (x < 0 || y < 0) return 0.0f; return (float)(x * 10 + y); }
static float Area(const V3& tl, const V3& br, int mode)
{
    float best = 0.0f, total = 0.0f, count = 0.0f;
    int sx = (int)tl.X + 1, sy = (int)tl.Y + 1, ex = (int)br.X, ey = (int)br.Y;
    V3 c; c.X = 0.0f; c.Y = 0.0f; c.Z = 0.0f;
    float samples[64]; int n = 0;
    samples[n++] = EdgeAt(tl); samples[n++] = EdgeAt(br);
    c.X = tl.X; c.Y = br.Y; samples[n++] = EdgeAt(c);
    c.X = br.X; c.Y = tl.Y; samples[n++] = EdgeAt(c);
    float yStart = (float)((int)tl.Y + 1), xEnd = (float)(int)br.X, yEnd = (float)(int)br.Y;
    for (c.X = (float)((int)tl.X + 1); c.X <= xEnd; c.X += 1.0f) { c.Y = tl.Y; samples[n++] = EdgeAt(c); c.Y = br.Y; samples[n++] = EdgeAt(c); }
    for (c.Y = yStart; c.Y <= yEnd; c.Y += 1.0f) { c.X = tl.X; samples[n++] = EdgeAt(c); c.X = br.X; samples[n++] = EdgeAt(c); }
    for (int x = sx; x <= ex; ++x) for (int y = sy; y <= ey; ++y) samples[n++] = BlockAt(x, y);
    best = samples[0];
    for (int i = 0; i < n; ++i) {
        float h = samples[i];
        if (i > 0 && h > best) best = h;
        if (h > 0.0f) { total += h; count += 1.0f; }
    }
    if (mode == 0) return best;
    if (count > 0.0f) return total / count;
    return 0.0f;
}
int main()
{
    V3 tl = { 1.5f, 2.5f, 0.0f }, br = { 4.25f, 4.75f, 0.0f };
    g_edgeCalls = g_blockCalls = 0;
    float r = Area(tl, br, 1);
    // edges: 4 corners + x steps 2,3,4 (x2) + y steps 3,4 (x2) = 14; blocks x 2..4, y 3..4 = 6
    if (g_edgeCalls != 14 || g_blockCalls != 6) { std::printf("calls %d %d 0063c230_TEST FAIL\n", g_edgeCalls, g_blockCalls); return 1; }
    float want = 33.5f;
    if (std::fabs(r - want) > 0.01f) { std::printf("%f 0063c230_TEST FAIL\n", r); return 1; }
    std::printf("0063c230_TEST PASS\n");
    return 0;
}
