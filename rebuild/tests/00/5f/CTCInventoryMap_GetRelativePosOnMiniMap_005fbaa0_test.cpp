// Behaviour test for 0x005FBAA0 CTCInventoryMap::GetRelativePosOnMiniMap: a copy of the row's logic over plain models of the
// maps (world position / size), the region (mini-map scale / offset) and the mini-map graphic's frame size.
#include <stdio.h>
#include <math.h>
#include <vector>
struct Coord { long X, Y; };
struct Vec2 { float X, Y; };
struct Vec3 { float X, Y, Z; };
struct MapModel { Coord Pos; long SizeX, SizeY; };
struct RegionModel { std::vector<long> Maps; float MiniMapScale; Vec2 MiniMapOffset; };
static MapModel g_maps[8];
static unsigned long g_frameW, g_frameH;
static void GetRelativePosOnMiniMap(RegionModel& region, const Vec3& position, Vec2& relative)
{
    std::vector<long> maps = region.Maps;
    Coord tl = g_maps[maps[0]].Pos;
    Coord br;
    br.X = g_maps[maps[0]].SizeX + tl.X;
    br.Y = g_maps[maps[0]].SizeY + tl.Y;
    for (unsigned int i = 1; i < maps.size(); ++i) {
        const MapModel& map = g_maps[maps[i]];
        if (tl.X > map.Pos.X) tl.X = map.Pos.X;
        long top = map.Pos.Y;
        if (tl.Y > top) tl.Y = top;
        if (br.X < map.Pos.X + map.SizeX) br.X = map.Pos.X + map.SizeX;
        if (br.Y < map.SizeY + top) br.Y = map.SizeY + top;
    }
    relative.X = position.X - tl.X;
    relative.Y = position.Y - tl.Y;
    long width = br.X - tl.X;
    long height = br.Y - tl.Y;
    relative.X /= (float)width;
    relative.Y /= (float)height;
    float scale[2];
    for (int k = 0; k < 2; ++k) scale[k] = 1.0f;
    if (width > height) scale[1] = (float)height / (float)width;
    else if (width < height) scale[0] = (float)width / (float)height;
    relative.X *= scale[0];
    relative.Y *= scale[1];
    float graphicWidth = (float)g_frameW;
    float graphicHeight = (float)g_frameH;
    relative.X = (relative.X * region.MiniMapScale - 0.5f) * graphicWidth;
    relative.Y = ((1.0f - relative.Y) * region.MiniMapScale - 0.5f) * graphicHeight;
    relative.X += region.MiniMapOffset.X * region.MiniMapScale;
    relative.Y += region.MiniMapOffset.Y * region.MiniMapScale;
}
static int near(float a, float b) { return fabs(a - b) < 1e-3f; }
int main()
{
    int ok = 1;
    // Two maps: [0,0]-[100,50] and [100,-50]-[200,50] -> bounds [0,-50]-[200,50], 200 x 100 (wider: Y scaled by 0.5).
    g_maps[3].Pos.X = 0; g_maps[3].Pos.Y = 0; g_maps[3].SizeX = 100; g_maps[3].SizeY = 50;
    g_maps[5].Pos.X = 100; g_maps[5].Pos.Y = -50; g_maps[5].SizeX = 100; g_maps[5].SizeY = 100;
    g_frameW = 256; g_frameH = 128;
    RegionModel r; r.Maps.push_back(3); r.Maps.push_back(5); r.MiniMapScale = 2.0f; r.MiniMapOffset.X = 10.0f; r.MiniMapOffset.Y = -4.0f;
    Vec3 p = { 50.0f, 0.0f, 7.0f }; Vec2 rel;
    GetRelativePosOnMiniMap(r, p, rel);
    // relX = 50/200 = 0.25 ; relY = 50/100 * 0.5 = 0.25 ; X = (0.25*2-0.5)*256 + 20 = 20 ; Y = ((1-0.25)*2-0.5)*128 - 8 = 120
    ok &= near(rel.X, 20.0f) && near(rel.Y, 120.0f);
    // Single taller map: [10,20] size 50 x 100 -> X scaled by 0.5.
    g_maps[1].Pos.X = 10; g_maps[1].Pos.Y = 20; g_maps[1].SizeX = 50; g_maps[1].SizeY = 100;
    RegionModel r2; r2.Maps.push_back(1); r2.MiniMapScale = 1.0f; r2.MiniMapOffset.X = 0.0f; r2.MiniMapOffset.Y = 0.0f;
    Vec3 p2 = { 35.0f, 70.0f, 0.0f };
    GetRelativePosOnMiniMap(r2, p2, rel);
    // relX = 25/50 * 0.5 = 0.25 -> (0.25-0.5)*256 = -64 ; relY = 50/100 = 0.5 -> (0.5-0.5)*128 = 0
    ok &= near(rel.X, -64.0f) && near(rel.Y, 0.0f);
    // Square map with unsigned frame size above 2^31 exercises the 2^32 fix-up path.
    g_maps[2].Pos.X = 0; g_maps[2].Pos.Y = 0; g_maps[2].SizeX = 10; g_maps[2].SizeY = 10;
    g_frameW = 0x80000000UL; g_frameH = 2;
    RegionModel r3; r3.Maps.push_back(2); r3.MiniMapScale = 1.0f; r3.MiniMapOffset.X = 0.0f; r3.MiniMapOffset.Y = 0.0f;
    Vec3 p3 = { 10.0f, 0.0f, 0.0f };
    GetRelativePosOnMiniMap(r3, p3, rel);
    ok &= near(rel.X / 2147483648.0f, 0.5f) && near(rel.Y, 1.0f);
    if (!ok) { printf("005fbaa0_TEST FAIL\n"); return 1; }
    printf("005fbaa0_TEST PASS\n");
    return 0;
}