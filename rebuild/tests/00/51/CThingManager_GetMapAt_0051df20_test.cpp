#include <cstdio>
// Behaviour test for CThingManager::GetMapAt (0x0051DF20): position truncated to a cell, world map queried.
struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long X, Y; };
struct CMap { long id; };
struct CWorldMap {
    CMap maps[4]; long lastX, lastY;
    CMap& GetMapAt(const C2DCoordI& p) { lastX = p.X; lastY = p.Y; return maps[(p.X + p.Y) & 3]; }
};
struct CThingManager {
    CWorldMap* WorldMap;
    CMap& GetMapAt(const C3DVector& pos)
    {
        C2DCoordI cell; cell.X = (long)pos.X; cell.Y = (long)pos.Y;
        return WorldMap->GetMapAt(cell);
    }
};
int main()
{
    CWorldMap w; for (int i = 0; i < 4; ++i) w.maps[i].id = i;
    CThingManager m; m.WorldMap = &w;
    C3DVector v; v.X = 2.9f; v.Y = -1.7f; v.Z = 100.0f;
    int fails = 0;
    CMap& r = m.GetMapAt(v);
    if (w.lastX != 2 || w.lastY != -1 || &r != &w.maps[1]) ++fails;
    v.X = 1.0f; v.Y = 2.0f;
    if (&m.GetMapAt(v) != &w.maps[3]) ++fails;
    if (fails) { std::printf("0051df20_TEST FAIL\n"); return 1; }
    std::printf("0051df20_TEST PASS\n");
    return 0;
}