// Behaviour test (0x0063a910): CWorldMap::SetGroundSizeZAtBlock sets the ground size on the owning map at the local coordinate when the position is changeable
#include <cstdio>
#include <cmath>
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
static int g_sets = 0; static long g_map = -1, g_lx = 0, g_ly = 0; static float g_size = 0.0f;
class CMap {
public:
    long Number;
    void SetGroundSizeZAt(const C2DCoordI& pos, float size) { ++g_sets; g_map = Number; g_lx = pos.X; g_ly = pos.Y; g_size = size; }
};
class CWorldMap {
public:
    CMap Maps[3];
    virtual long GetMapNumberAt(const C2DCoordI& pos) const { return pos.X < 100 ? 1 : 2; }
    virtual CMap& GetMap(long map) { return Maps[map]; }
    virtual bool IsPosChangeable(const C2DCoordI& pos) const { return pos.Y != 13; }
    virtual ~CWorldMap() { }
    bool IsPosInMap(const C2DCoordI& pos) const { return pos.X >= 0; }
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const { return C2DCoordI(pos.X - 100 * (map - 1), pos.Y); }
    void SetGroundSizeZAtBlock(const C2DCoordI& pos, float size)
    {
        if (IsPosInMap(pos))
        {
            long map = GetMapNumberAt(pos);
            if (map && IsPosChangeable(pos))
            {
                C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, pos);
                GetMap(map).SetGroundSizeZAt(local, size);
            }
        }
    }
};
int main()
{
    CWorldMap wm;
    for (int i = 0; i < 3; ++i) wm.Maps[i].Number = i;
    wm.SetGroundSizeZAtBlock(C2DCoordI(-1, 0), 1.0f);     // outside
    wm.SetGroundSizeZAtBlock(C2DCoordI(5, 13), 1.0f);     // not changeable
    if (g_sets != 0) { std::printf("0063a910_TEST FAIL\n"); return 1; }
    wm.SetGroundSizeZAtBlock(C2DCoordI(150, 7), 2.5f);
    if (g_sets != 1 || g_map != 2 || g_lx != 50 || g_ly != 7 || std::fabs(g_size - 2.5f) > 1e-4f) { std::printf("0063a910_TEST FAIL\n"); return 1; }
    std::printf("0063a910_TEST PASS\n");
    return 0;
}