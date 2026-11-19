// Behaviour test for CMiniMapDisplay::GetDrawPosition (0x0082A020), a portable model of the row. The world position is made relative
// to the region map (origin, extents), scaled by the relative map size (the long side is 1), placed in the render box after the box is
// scaled about its top-left by the region's minimap scale, floored to whole pixels (Y measured up from the box bottom), then shifted by
// (GDummyInt + region minimap offset) / (256 / box size).
#include <stdio.h>
#include <math.h>
struct Region { float MiniMapScale, OffX, OffY; };
struct MapInfo { long OX, OY, EX, EY; };
static long Floor(float f) { return (long)floor(f); }
static void RelativeMapSize(const MapInfo& m, float& sx, float& sy)
{
    if (m.EX == m.EY) { sx = 1.0f; sy = 1.0f; }
    else if (m.EX > m.EY) { sx = 1.0f; sy = (float)m.EY / m.EX; }
    else { sx = (float)m.EX / m.EY; sy = 1.0f; }
}
static void GetDrawPosition(const MapInfo& m, const Region& r, long d1, long d2, float px, float py,
                            float tlx, float tly, float brx, float bry, float& ox, float& oy)
{
    float tx = (px - m.OX) / m.EX, ty = (py - m.OY) / m.EY;
    float sx, sy; RelativeMapSize(m, sx, sy);
    brx = ((brx - tlx) * r.MiniMapScale) + tlx;
    bry = ((bry - tly) * r.MiniMapScale) + tly;
    float w = brx - tlx, h = bry - tly;
    ox = (float)Floor((sx * tx * w) + tlx);
    oy = (float)Floor((tly + h) - (sy * ty * h));
    ox += (d1 + r.OffX) / (256.0f / w);
    oy += (d2 + r.OffY) / (256.0f / h);
}
int main()
{
    int bad = 0; float x, y;
    MapInfo sq = { 0, 0, 100, 100 }; Region r1 = { 1.0f, 0.0f, 0.0f };
    GetDrawPosition(sq, r1, 0, 0, 50.0f, 25.0f, 10.0f, 20.0f, 110.0f, 120.0f, x, y);   // centre-left of a square map
    if (x != 60.0f || y != 95.0f) ++bad;
    MapInfo wide = { -100, 0, 200, 100 }; Region r2 = { 0.5f, 0.0f, 0.0f };
    GetDrawPosition(wide, r2, 0, 0, 100.0f, 50.0f, 0.0f, 0.0f, 256.0f, 256.0f, x, y); // wide map: Y size 0.5; box halved to 128
    if (x != 128.0f || y != 96.0f) ++bad;
    GetDrawPosition(sq, r1, 0, 0, 33.3f, 66.6f, 0.0f, 0.0f, 10.0f, 10.0f, x, y);      // floor to whole pixels
    if (x != 3.0f || y != 3.0f) ++bad;
    Region r3 = { 1.0f, 6.0f, -2.0f };
    GetDrawPosition(sq, r3, 2, 4, 0.0f, 0.0f, 0.0f, 0.0f, 128.0f, 64.0f, x, y);       // offset: (2+6)/(256/128), (4-2)/(256/64)
    if (x != 4.0f || y != 64.5f) ++bad;
    MapInfo tall = { 0, 0, 50, 100 };
    GetDrawPosition(tall, r1, 0, 0, 50.0f, 0.0f, 0.0f, 0.0f, 100.0f, 100.0f, x, y);   // tall map: X size 0.5
    if (x != 50.0f || y != 100.0f) ++bad;
    printf(bad ? "0082a020_TEST FAIL\n" : "0082a020_TEST PASS\n");
    return bad ? 1 : 0;
}
