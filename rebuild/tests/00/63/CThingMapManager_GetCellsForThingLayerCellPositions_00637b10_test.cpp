// Behaviour test for CThingMapManager::GetCellsForThingLayerCellPositions (0x00637B10): each corner's cell resolved
// from its map and layer, a corner that repeats an earlier one (same map and position) left null.
#include <cstdio>

struct CThingMapCell { int Id; };
struct CThingMap {
    long ExtentsX[2];
    CThingMapCell Cells[2][16];
    CThingMapCell& GetCell(unsigned char level, long x, long y) { return Cells[level][ExtentsX[level] * y + x]; }
};
class C2DCoordI { public: long X, Y; bool operator==(const C2DCoordI& o) const { return X == o.X && Y == o.Y; } };
struct CThingMapInfo { CThingMap* PThingMap; long RefCount; };
struct CMapCellPosInfo { CThingMapInfo* PMapInfo; C2DCoordI LocalPos; CThingMapCell* PCell; };
struct CMapCellPosInfoGroup { long Layer; CMapCellPosInfo TLPos, TRPos, BLPos, BRPos; };
inline CThingMapCell* GetMapCell(CThingMapInfo* info, long layer, const C2DCoordI& pos) { return &info->PThingMap->GetCell((unsigned char)layer, pos.X, pos.Y); }
static void Resolve(CMapCellPosInfoGroup* group)
{
    CThingMapInfo* tl = group->TLPos.PMapInfo;
    group->TLPos.PCell = 0;
    if (tl) group->TLPos.PCell = GetMapCell(tl, group->Layer, group->TLPos.LocalPos);
    CThingMapInfo* tr = group->TRPos.PMapInfo;
    group->TRPos.PCell = 0;
    if (tr && !(tr == tl && group->TRPos.LocalPos == group->TLPos.LocalPos)) group->TRPos.PCell = GetMapCell(tr, group->Layer, group->TRPos.LocalPos);
    CThingMapInfo* bl = group->BLPos.PMapInfo;
    group->BLPos.PCell = 0;
    if (bl && !(bl == tl && group->BLPos.LocalPos == group->TLPos.LocalPos)) group->BLPos.PCell = GetMapCell(bl, group->Layer, group->BLPos.LocalPos);
    CThingMapInfo* br = group->BRPos.PMapInfo;
    group->BRPos.PCell = 0;
    if (br && !(br == bl && group->BRPos.LocalPos == group->BLPos.LocalPos) && !(br == tr && group->BRPos.LocalPos == group->TRPos.LocalPos))
        group->BRPos.PCell = GetMapCell(br, group->Layer, group->BRPos.LocalPos);
}
static void Set(CMapCellPosInfo& p, CThingMapInfo* i, long x, long y) { p.PMapInfo = i; p.LocalPos.X = x; p.LocalPos.Y = y; p.PCell = (CThingMapCell*)1; }

int main()
{
    CThingMap map; map.ExtentsX[0] = 4; map.ExtentsX[1] = 2;
    CThingMapInfo info = { &map, 1 };
    CMapCellPosInfoGroup g; g.Layer = 1;
    Set(g.TLPos, &info, 0, 0); Set(g.TRPos, &info, 1, 0); Set(g.BLPos, &info, 0, 0); Set(g.BRPos, &info, 1, 0);
    Resolve(&g);
    bool ok = g.TLPos.PCell == &map.Cells[1][0] && g.TRPos.PCell == &map.Cells[1][1] && g.BLPos.PCell == 0 && g.BRPos.PCell == 0;
    g.Layer = 0;
    Set(g.TLPos, 0, 0, 0); Set(g.TRPos, &info, 3, 0); Set(g.BLPos, &info, 0, 2); Set(g.BRPos, &info, 3, 2);
    Resolve(&g);
    ok = ok && g.TLPos.PCell == 0 && g.TRPos.PCell == &map.Cells[0][3] && g.BLPos.PCell == &map.Cells[0][8] && g.BRPos.PCell == &map.Cells[0][11];
    if (!ok) { std::printf("00637b10_TEST FAIL\n"); return 1; }
    std::printf("00637b10_TEST PASS\n");
    return 0;
}