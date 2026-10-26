// behaviour test for CThingMapManager::GetThingLayerCellPositions (0x00637E70): layer stored, four truncated corners handed to InitPos in TL, TR, BL, BR order
#include <cstdio>
class C2DVector { public: float X, Y; };
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) {} };
struct CMapCellPosInfo { void* PMapInfo; C2DCoordI LocalPos; void* PCell; CMapCellPosInfo() : PMapInfo(0), LocalPos(0, 0), PCell(0) {} };
struct CMapCellPosInfoGroup { long Layer; CMapCellPosInfo TLPos, TRPos, BLPos, BRPos; };
static int g_order = 0;
struct CThingMapManager {
    void InitPos(const C2DCoordI& pos, CMapCellPosInfo* info, long layer) { info->LocalPos = pos; info->PCell = (void*)(long)(++g_order * 100 + layer); }
    void GetThingLayerCellPositions(float radius, const C2DVector& pos, long layer, CMapCellPosInfoGroup* group)
    {
        group->Layer = layer;
        C2DCoordI topLeft((long)(pos.X - radius), (long)(pos.Y - radius));
        C2DCoordI bottomRight((long)(radius + pos.X), (long)(radius + pos.Y));
        C2DCoordI bottomLeft(topLeft.X, bottomRight.Y);
        C2DCoordI topRight(bottomRight.X, topLeft.Y);
        InitPos(topLeft, &group->TLPos, layer); InitPos(topRight, &group->TRPos, layer);
        InitPos(bottomLeft, &group->BLPos, layer); InitPos(bottomRight, &group->BRPos, layer);
    } };
int main()
{
    CThingMapManager m; CMapCellPosInfoGroup g; C2DVector p = { 10.7f, 20.2f };
    m.GetThingLayerCellPositions(3.5f, p, 2, &g);
    int bad = g.Layer != 2;
    if (g.TLPos.LocalPos.X != 7 || g.TLPos.LocalPos.Y != 16 || g.BRPos.LocalPos.X != 14 || g.BRPos.LocalPos.Y != 23) bad = 1;
    if (g.TRPos.LocalPos.X != 14 || g.TRPos.LocalPos.Y != 16 || g.BLPos.LocalPos.X != 7 || g.BLPos.LocalPos.Y != 23) bad = 1;
    if ((long)g.TLPos.PCell != 102 || (long)g.TRPos.PCell != 202 || (long)g.BLPos.PCell != 302 || (long)g.BRPos.PCell != 402) bad = 1;
    if (bad) { std::printf("00637e70_TEST FAIL\n"); return 1; }
    std::printf("00637e70_TEST PASS\n");
    return 0;
}