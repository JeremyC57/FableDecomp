#include <cstdio>
// Behaviour test: IsToBePlacedInMapwho -- the layer from the thing's physics radius, the four corner cells of that layer
// at the position; true when any corner lies in a thing map.
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
struct C3DVector { float X, Y, Z; };
struct CThingMapInfo { int Id; };
struct CMapCellPosInfo { CThingMapInfo* PMapInfo; long LocalX, LocalY; void* PCell; };
struct CMapCellPosInfoGroup { long Layer; CMapCellPosInfo TLPos, TRPos, BLPos, BRPos; };
struct CTCPhysicsBase { float Radius; };
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CTCMapwho { CThing* ParentThing; };
static CThingMapInfo g_map = { 1 };
static int g_corner = -1;   // which corner the stand-in places inside a map (-1: none)
static float g_seenRadius = 0; static long g_seenLayer = -1; static float g_seenX = 0, g_seenY = 0;
struct CThingMapManager {
    unsigned char GetThingMapwhoLevel(float radius) const { return radius > 4.0f ? 2 : 1; }   // 0x006378E0 stand-in
    void GetThingLayerCellPositions(float radius, const C2DVector& pos, long layer, CMapCellPosInfoGroup& group) const
    {
        g_seenRadius = radius; g_seenLayer = layer; g_seenX = pos.X; g_seenY = pos.Y;
        group.Layer = layer;
        group.TLPos.PMapInfo = g_corner == 0 ? &g_map : 0;
        group.TRPos.PMapInfo = g_corner == 1 ? &g_map : 0;
        group.BLPos.PMapInfo = g_corner == 2 ? &g_map : 0;
        group.BRPos.PMapInfo = g_corner == 3 ? &g_map : 0;
    }
    bool IsToBePlacedInMapwho(const CTCMapwho& mapwho, const C3DVector& pos) const
    {
        float radius = mapwho.ParentThing->PhysicsTC->Radius;
        long layer = GetThingMapwhoLevel(radius);
        CMapCellPosInfoGroup group;
        GetThingLayerCellPositions(radius, C2DVector(pos.X, pos.Y), layer, group);
        if (group.TLPos.PMapInfo || group.TRPos.PMapInfo || group.BLPos.PMapInfo || group.BRPos.PMapInfo)
            return true;
        return false;
    }
};
int main()
{
    CTCPhysicsBase phys = { 6.0f };
    CThing thing = { &phys };
    CTCMapwho mw = { &thing };
    C3DVector pos = { 10.0f, 20.0f, 30.0f };
    CThingMapManager m;
    if (m.IsToBePlacedInMapwho(mw, pos)) { std::printf("00637f40_TEST FAIL\n"); return 1; }
    if (g_seenRadius != 6.0f || g_seenLayer != 2 || g_seenX != 10.0f || g_seenY != 20.0f) { std::printf("00637f40_TEST FAIL\n"); return 1; }
    for (g_corner = 0; g_corner < 4; ++g_corner)
        if (!m.IsToBePlacedInMapwho(mw, pos)) { std::printf("00637f40_TEST FAIL\n"); return 1; }
    phys.Radius = 1.0f; g_corner = -1;
    m.IsToBePlacedInMapwho(mw, pos);
    if (g_seenLayer != 1) { std::printf("00637f40_TEST FAIL\n"); return 1; }
    std::printf("00637f40_TEST PASS\n");
    return 0;
}