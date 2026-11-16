// Behaviour test for 0x0066CF00 CRegionMinimap::PopulateRegionMinimap: every cell maps its world position (top-left + cell / size *
// square extent) to the strongest engine theme; inside the map the engine theme's EMinimapThemeType 1..8 becomes region theme
// 1, 6, 4, 5, 2, 7, 3, 8 (anything else, or outside the map, 0); a theme whose lighting flag is set takes the illumination value,
// the rest 0. Then each building's bounding square (centre +- radius) is clipped to the region box (else-if ClipTo), turned into a
// TGA cell range grown by one cell each way inside the minimap, and every cell of that range whose ray hits the mesh becomes theme 8.
// A model of the row's steps on a 10x10 minimap over the region (0,0)-(100,100).
#include <cstdio>

struct Box { float TLX, TLY, BRX, BRY; };
static void ClipTo(Box& b, const Box& o)
{
    if (b.TLX < o.TLX) b.TLX = o.TLX; else if (b.TLX > o.BRX) b.TLX = o.BRX;
    if (b.TLY < o.TLY) b.TLY = o.TLY; else if (b.TLY > o.BRY) b.TLY = o.BRY;
    if (b.BRX < o.TLX) b.BRX = o.TLX; else if (b.BRX > o.BRX) b.BRX = o.BRX;
    if (b.BRY < o.TLY) b.BRY = o.TLY; else if (b.BRY > o.BRY) b.BRY = o.BRY;
}

static const long kSize = 10;
static const long kTLX = 0, kTLY = 0, kSquare = 100;
static long theme[kSize][kSize];
static float light[kSize][kSize];
static long gRange[4];                                                 // the grown TGA cell range of the building
static const bool kUseLighting[9] = { false, true, false, true, false, true, false, true, false };

static long EngineThemeAt(float x, float y) { return (long)(x / 10.0f); }          // 0..9 across the map
static bool IsPosInMap(float x, float y) { return y < 90.0f; }                       // the last row is outside
static float Illuminate(float x, float y) { return x + y; }
static bool MeshHit(float x, float y) { return x >= 60.0f && y < 30.0f; }

static void Populate(float bx, float by, float radius)
{
    for (long py = 0; py < kSize; ++py)
        for (long px = 0; px < kSize; ++px) {
            float wx = kTLX + ((float)px / (float)kSize) * kSquare, wy = kTLY + (float)py / (float)kSize * kSquare;
            long t = 0;
            if (IsPosInMap(wx, wy)) {
                switch (EngineThemeAt(wx, wy)) {
                case 1: t = 1; break; case 2: t = 6; break; case 3: t = 4; break; case 4: t = 5; break;
                case 5: t = 2; break; case 6: t = 7; break; case 7: t = 3; break; case 8: t = 8; break;
                default: t = 0; break;
                }
            }
            theme[py][px] = t;
            light[py][px] = kUseLighting[t] ? Illuminate(wx, wy) : 0.0f;
        }
    Box region = { (float)kTLX, (float)kTLY, (float)(kTLX + kSquare), (float)(kTLY + kSquare) };
    Box b = { bx - radius, by - radius, bx + radius, by + radius };
    ClipTo(b, region);
    long tlx = (long)((b.TLX - kTLX) / kSquare * kSize), tly = (long)((b.TLY - kTLY) / kSquare * kSize);
    long brx = (long)((b.BRX - kTLX) / kSquare * kSize), bry = (long)((b.BRY - kTLY) / kSquare * kSize);
    if (tlx > 0) --tlx;
    if (tly > 0) --tly;
    if (brx < kSize) ++brx;
    if (bry < kSize) ++bry;
    gRange[0] = tlx; gRange[1] = tly; gRange[2] = brx; gRange[3] = bry;
    for (long y = tly; y < bry; ++y)
        for (long x = tlx; x < brx; ++x) {
            float wx = kTLX + ((float)x / (float)kSize) * kSquare, wy = kTLY + (float)y / (float)kSize * kSquare;
            if (MeshHit(wx, wy)) theme[y][x] = 8;
        }
}

int main()
{
    Populate(95.0f, 5.0f, 20.0f);   // square (75,-15)-(115,25) -> clipped (75,0)-(100,25) -> cells x 6..9, y 0..2 after growth
    bool ok = true;
    static const long row5[kSize] = { 0, 1, 6, 4, 5, 2, 7, 3, 8, 0 };
    for (long x = 0; x < kSize; ++x) ok = ok && theme[5][x] == row5[x];
    for (long x = 0; x < kSize; ++x) ok = ok && theme[9][x] == 0;            // outside the map
    ok = ok && light[5][1] == 60.0f && light[5][2] == 0.0f && light[5][3] == 0.0f && light[5][6] == 110.0f && light[5][7] == 120.0f && light[9][1] == 0.0f;
    ok = ok && gRange[0] == 6 && gRange[1] == 0 && gRange[2] == 10 && gRange[3] == 3;
    ok = ok && theme[0][6] == 8 && theme[2][9] == 8 && theme[0][5] == 2 && theme[3][6] == 7;   // grown range, mesh test, untouched cells
    std::printf(ok ? "0066cf00_TEST PASS\n" : "0066cf00_TEST FAIL\n");
    return ok ? 0 : 1;
}