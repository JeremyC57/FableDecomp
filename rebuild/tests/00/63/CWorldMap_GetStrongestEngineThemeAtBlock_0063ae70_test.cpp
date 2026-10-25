// Behaviour test (0x0063ae70): CWorldMap::GetStrongestEngineThemeAtBlock asks the loaded map under the position, else returns 0
#include <cstdio>
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
class CMap {
public:
    long Theme;
    long GetStrongestEngineThemeAt(const C2DCoordI& pos, long& strength) const { strength = pos.X + pos.Y; return Theme; }
};
class CWorldMap {
public:
    CMap Maps[3];
    bool Loaded[3];
    virtual bool IsMapLoaded(long map) const { return Loaded[map]; }
    virtual long GetMapNumberAt(const C2DCoordI& pos) const { return pos.X < 0 ? 0 : (pos.X < 100 ? 1 : 2); }
    virtual const CMap& PeekMap(long map) const { return Maps[map]; }
    virtual ~CWorldMap() { }
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const { return C2DCoordI(pos.X - 100 * (map - 1), pos.Y); }
    long GetStrongestEngineThemeAtBlock(const C2DCoordI& pos, long& strength) const
    {
        long map = GetMapNumberAt(pos);
        if (map && IsMapLoaded(map))
        {
            C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            return PeekMap(map).GetStrongestEngineThemeAt(local, strength);
        }
        return 0;
    }
};
int main()
{
    CWorldMap wm;
    wm.Maps[1].Theme = 11; wm.Maps[2].Theme = 22;
    wm.Loaded[0] = true; wm.Loaded[1] = false; wm.Loaded[2] = true;
    long strength = -1;
    if (wm.GetStrongestEngineThemeAtBlock(C2DCoordI(-5, 0), strength) != 0 || strength != -1) { std::printf("0063ae70_TEST FAIL\n"); return 1; }
    if (wm.GetStrongestEngineThemeAtBlock(C2DCoordI(50, 0), strength) != 0 || strength != -1) { std::printf("0063ae70_TEST FAIL\n"); return 1; }
    if (wm.GetStrongestEngineThemeAtBlock(C2DCoordI(130, 4), strength) != 22 || strength != 34) { std::printf("0063ae70_TEST FAIL\n"); return 1; }
    std::printf("0063ae70_TEST PASS\n");
    return 0;
}