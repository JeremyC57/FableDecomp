#include <cstdio>
#include <cmath>
// Behaviour test for CMap::DrawGetSizeZAt (0x008F7630): clamped corners and the parity-split triangle interpolation.
struct CGrid {
    float H[5][5];                                 // H[y][x]
    long W, Hh;
};
#define CLAMP_TO_MAP(v, high) ((v) < 0 ? 0 : ((v) > (high) ? (high) : (v)))
static float SizeZAt(const CGrid& g, float fx, float fy)
{
    long x = (long)fx; float dx = fx - x;
    long y = (long)fy; float dy = fy - y;
    long x0 = CLAMP_TO_MAP(x, g.W - 1), y1 = CLAMP_TO_MAP(y + 1, g.Hh - 1);
    long x1 = CLAMP_TO_MAP(x + 1, g.W - 1), y0 = CLAMP_TO_MAP(y, g.Hh - 1);
    float h01 = g.H[y1][x0], h11 = g.H[y1][x1], h10 = g.H[y0][x1], h00 = g.H[y0][x0];
    float z;
    if ((x + y) & 1) {
        if (dx + dy > 1.0f) z = (h10 - h11) * (1.0f - dy) + (h01 - h11) * (1.0f - dx) + h11;
        else z = (h01 - h00) * dy + (h10 - h00) * dx + h00;
    } else if (dx > dy) z = (h11 - h10) * dy + (h10 - h00) * dx + h00;
    else z = (h01 - h00) * dy + (h11 - h01) * dx + h00;
    return z;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static int Fail(int n) { std::printf("008f7630_TEST FAIL %d\n", n); return 1; }
int main()
{
    CGrid g;
    g.W = 5; g.Hh = 5;
    for (int y = 0; y < 5; y++) for (int x = 0; x < 5; x++) g.H[y][x] = (float)(2 * x + 3 * y);   // a plane
    if (!Near(SizeZAt(g, 1.25f, 2.5f), 2 * 1.25f + 3 * 2.5f)) return Fail(1);     // odd cell, lower triangle
    if (!Near(SizeZAt(g, 1.75f, 2.5f), 2 * 1.75f + 3 * 2.5f)) return Fail(2);     // odd cell, upper triangle
    if (!Near(SizeZAt(g, 2.75f, 2.25f), 2 * 2.75f + 3 * 2.25f)) return Fail(3);   // even cell, dx > dy
    if (!Near(SizeZAt(g, 2.25f, 2.75f), 2 * 2.25f + 3 * 2.75f)) return Fail(4);   // even cell, dx <= dy
    if (!Near(SizeZAt(g, 4.5f, 4.5f), g.H[4][4])) return Fail(5);                   // clamped to the last corner
    g.H[1][1] = 10.0f; g.H[1][2] = 0; g.H[2][1] = 0; g.H[2][2] = 0;
    if (!Near(SizeZAt(g, 1.0f, 1.0f), 10.0f)) return Fail(6);
    std::printf("008f7630_TEST PASS\n");
    return 0;
}
