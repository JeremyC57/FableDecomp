// Behaviour test for CWorldMap::GetPGameMapCellAt 0x0063D8A0: world pos -> local pos /4 -> bottom-up cell index.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
struct CGameMapCell { unsigned char AtmosThemes[3]; unsigned char AtmosBlends[2]; signed char SoundTheme; unsigned char Music; };
struct CGameMap {
    CGameMapCell* Cells; long SizeX; long SizeY;
    const CGameMapCell* PeekCell(const C2DCoordI& pos) const { return &Cells[(SizeY - pos.Y - 1) * SizeX + pos.X]; }
};
struct CMap { CGameMap* GameMap; };
static bool g_loaded = true;
struct CWorldMap {
    CMap Map1;
    virtual bool IsMapLoaded(long) const { return g_loaded; }
    virtual long GetMapNumberAt(const C2DCoordI& pos) const { return pos.X >= 1000 && pos.X < 1100 ? 1 : 0; }
    virtual const CMap& PeekMap(long) const { return Map1; }
    virtual ~CWorldMap() {}
    bool IsPosInMap(const C3DVector& pos) const { return pos.X >= 0 && pos.Y >= 0; }
    C3DVector ConvertWorldMapCoordsToLocalCoords(long, const C3DVector& pos) const
    { C3DVector r; r.X = pos.X - 1000.0f; r.Y = pos.Y - 2000.0f; r.Z = pos.Z; return r; }
    const CGameMapCell* GetPGameMapCellAt(const C3DVector& pos) const;
};

const CGameMapCell* CWorldMap::GetPGameMapCellAt(const C3DVector& pos) const
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y));
        if (map && IsMapLoaded(map))
        {
            C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& gameMap = PeekMap(map);
            return gameMap.GameMap->PeekCell(C2DCoordI((long)(local.X * 0.25f), (long)(local.Y * 0.25f)));
        }
    }
    return 0;
}

int main()
{
    CGameMapCell cells[25 * 25]; CGameMap gm; gm.Cells = cells; gm.SizeX = 25; gm.SizeY = 25;
    CWorldMap w; w.Map1.GameMap = &gm; bool ok = true;
    C3DVector p = { 1013.0f, 2009.5f, 0.0f };      // local (13, 9.5) -> cell (3, 2) -> row 25-2-1 = 22
    if (w.GetPGameMapCellAt(p) != &cells[22 * 25 + 3]) ok = false;
    C3DVector q = { 500.0f, 2000.0f, 0.0f };       // map 0
    if (w.GetPGameMapCellAt(q) != 0) ok = false;
    C3DVector r = { -1.0f, 5.0f, 0.0f };           // outside the world
    if (w.GetPGameMapCellAt(r) != 0) ok = false;
    g_loaded = false; if (w.GetPGameMapCellAt(p) != 0) ok = false;
    if (!ok) { std::printf("0063d8a0_TEST FAIL\n"); return 1; }
    std::printf("0063d8a0_TEST PASS\n");
    return 0;
}
