// Behaviour test for CEngineMap::PeekWaterHeight (0x00B41A80), a portable model of the row: the water depth at (x, y); at most 0.001 ->
// 0; else the landscape height at (x, y) plus the depth, where the landscape height comes from the neighbouring map that holds the cell
// when (x, y) is off this map (or this map has no game map), else from this map's game map at the cell clamped to its edges, snapped
// to 1/128 (round to nearest).
#include <stdio.h>
#include <math.h>
struct Map {
    long W, H, WorldX, WorldY; bool HasGame; float Base; const Map* Neighbour; float Depth;
    float Ground(long x, long y) const { return Base + x * 0.25f + y * 0.125f + 0.001f; }   // stand-in DrawBlockGetSizeZAt
    float PeekLandscapeHeight(long x, long y) const
    {
        if (x >= W || y >= H || x < 0 || y < 0 || !HasGame) {
            const Map* m = Neighbour;
            if (m) {
                long my = WorldY - m->WorldY + y, mx = WorldX - m->WorldX + x;
                if (mx < m->W && my < m->H && mx >= 0 && my >= 0 && m->HasGame) return m->PeekLandscapeHeight(mx, my);
            }
            if (x < 0) x = 0; else if (x > W - 1) x = W - 1;
            if (y < 0) y = 0; else if (y > H - 1) y = H - 1;
        }
        if (HasGame) return (float)floor(Ground(x, y) * 128.0f + 0.5f) * (1.0f / 128.0f);
        return 0.0f;
    }
    float PeekWaterHeight(long x, long y) const
    {
        float depth = Depth;
        return depth > 0.001f ? PeekLandscapeHeight(x, y) + depth : 0.0f;
    }
};
int main()
{
    int bad = 0;
    Map east = { 8, 8, 8, 0, true, 10.0f, 0, 2.0f };
    Map self = { 8, 8, 0, 0, true, 1.0f, &east, 2.0f };
    if (self.PeekWaterHeight(2, 4) != 1.0f + 0.5f + 0.5f + 2.0f) ++bad;             // own cell: 1 + 2/4 + 4/8 (+0.001 snapped away) + depth
    if (self.PeekWaterHeight(9, 0) != 10.0f + 0.25f + 2.0f) ++bad;                  // world x 9 is the east map's cell 1
    self.Neighbour = 0;
    if (self.PeekWaterHeight(9, -3) != 1.0f + 7 * 0.25f + 2.0f) ++bad;              // no neighbour: clamped to (7, 0)
    self.Depth = 0.001f;
    if (self.PeekWaterHeight(2, 4) != 0.0f) ++bad;                                  // depth not above 0.001
    self.Depth = 0.5f; self.HasGame = false;
    if (self.PeekWaterHeight(2, 4) != 0.5f) ++bad;                                  // no game map and no neighbour: height 0
    printf(bad ? "00b41a80_TEST FAIL %d\n" : "00b41a80_TEST PASS\n", bad);
    return bad != 0;
}
