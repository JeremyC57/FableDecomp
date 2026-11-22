// 00637fc0 NThingMap::CThingMapManager::PlaceThing
// Leaf: ?PlaceThing@CThingMapManager@NThingMap@@QAEXAAVCTCMapwho@@ABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_CTHINGMAPMANAGER_PLACE_PASS27
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_NODE
#define FABLE_RETAIL_CTCMAPWHO_SETINMAP
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGMAPMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TO2D
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_C2DCOORDI_NOTEQUAL
#define FABLE_RETAIL_C2DCOORDI_ORIGINAL_COMPARE_PASS27
#include "fable_assert.h"
#include "engine/CThing.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingMapManager.h"

inline const C2DBoxI& NThingMap::CThingMap::GetArea() const { return Area; }

namespace NThingMap {

void CThingMapManager::PlaceThing(CTCMapwho& mapwho,const C3DVector& pos)
{
    CThing& thing = mapwho.GetParentThing();
    if (!thing.IsLocal()) {
        FABLE_RETAIL_ASSERT(!mapwho.IsInMap());
        CTCPhysicsBase* ptc_physics = thing.GetPhysicsTC();
        float radius = ptc_physics->GetRadius();
        long mapwho_level = GetThingMapwhoLevel(radius);
        CMapCellPosInfoGroup mapwho_pos;
        GetThingLayerCellPositions(radius,pos.ToC2DVector(),mapwho_level,mapwho_pos);
        GetCellsForThingLayerCellPositions(mapwho_pos);
        if (mapwho_pos.TLPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.TLPos.PCell,mapwho.GetTLNode());
        if (mapwho_pos.TRPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.TRPos.PCell,mapwho.GetTRNode());
        if (mapwho_pos.BRPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.BRPos.PCell,mapwho.GetBRNode());
        if (mapwho_pos.BLPos.PCell)
            AddMapwhoNodeToCell(*mapwho_pos.BLPos.PCell,mapwho.GetBLNode());
        mapwho.SetInMap(true);
        ++NoThings;
    }
}

void CThingMapManager::GetThingLayerCellPositions(float radius, const C2DVector& in_pos, long level, CMapCellPosInfoGroup& cells) const
{
    long scale_shift = LayerInfo[level].ScaleShift;
    cells.Layer = level;
    C2DCoordI tlpos((long)(in_pos.X-radius),(long)(in_pos.Y-radius));
    C2DCoordI brpos((long)(radius+in_pos.X),(long)(radius+in_pos.Y));
    C2DCoordI blpos(tlpos.X,brpos.Y);
    C2DCoordI trpos(brpos.X,tlpos.Y);
    InitPos(tlpos,cells.TLPos,level);
    InitPos(trpos,cells.TRPos,level);
    InitPos(blpos,cells.BLPos,level);
    InitPos(brpos,cells.BRPos,level);
}

void CThingMapManager::GetCellsForThingLayerCellPositions(CMapCellPosInfoGroup& cells)
{
    cells.TLPos.PCell = 0;
    if (cells.TLPos.PMapInfo)
        cells.TLPos.PCell = &GetCell(cells.TLPos.PMapInfo,cells.Layer,cells.TLPos.LocalPos);
    cells.TRPos.PCell = 0;
    if (cells.TRPos.PMapInfo && (cells.TRPos.PMapInfo != cells.TLPos.PMapInfo || cells.TRPos.LocalPos != cells.TLPos.LocalPos))
        cells.TRPos.PCell = &GetCell(cells.TRPos.PMapInfo,cells.Layer,cells.TRPos.LocalPos);
    cells.BLPos.PCell = 0;
    if (cells.BLPos.PMapInfo && (cells.BLPos.PMapInfo != cells.TLPos.PMapInfo || cells.BLPos.LocalPos != cells.TLPos.LocalPos))
        cells.BLPos.PCell = &GetCell(cells.BLPos.PMapInfo,cells.Layer,cells.BLPos.LocalPos);
    cells.BRPos.PCell = 0;
    if (cells.BRPos.PMapInfo && (cells.BRPos.PMapInfo != cells.BLPos.PMapInfo || cells.BRPos.LocalPos != cells.BLPos.LocalPos) && (cells.BRPos.PMapInfo != cells.TRPos.PMapInfo || cells.BRPos.LocalPos != cells.TRPos.LocalPos))
        cells.BRPos.PCell = &GetCell(cells.BRPos.PMapInfo,cells.Layer,cells.BRPos.LocalPos);
}
inline CThingMapCell& CThingMapManager::GetCell(const CThingMapInfo* pmap, long layer, const C2DCoordI& pos)
{
    FABLE_RETAIL_ASSERT(pmap->RefCount);
    return pmap->PThingMap->GetCell(static_cast<unsigned char>(layer), pos.X, pos.Y);
}

inline const CThingMapManager::CThingMapInfo* CThingMapManager::PeekMapInfoAt(const C2DCoordI& pos) const
{
    return &ThingMaps[pos.X / 128 + pos.Y / 128 * Dimensions.X];
}

void CThingMapManager::InitPos(const C2DCoordI& pos, CMapCellPosInfo& pos_info, long layer) const
{
    long scale_shift = LayerInfo[layer].ScaleShift;
    pos_info.PMapInfo = PeekMapInfoAt(pos);
    if (scale_shift == -1) {
        if (pos_info.PMapInfo && pos_info.PMapInfo->RefCount > 0) {
            pos_info.LocalPos.X = 0;
            pos_info.LocalPos.Y = 0;
        } else {
            pos_info.PMapInfo = 0;
        }
    } else if (pos_info.PMapInfo && pos_info.PMapInfo->RefCount > 0) {
        const C2DBoxI& map_area = pos_info.PMapInfo->PThingMap->GetArea();
        pos_info.LocalPos.X = (pos.X - map_area.TLX) >> scale_shift;
        pos_info.LocalPos.Y = (pos.Y - map_area.TLY) >> scale_shift;
        FABLE_RETAIL_ASSERT(pos_info.LocalPos.X >= 0 && pos_info.LocalPos.X < pos_info.PMapInfo->PThingMap->GetLayerExtents(layer).X);
        FABLE_RETAIL_ASSERT(pos_info.LocalPos.Y >= 0 && pos_info.LocalPos.Y < pos_info.PMapInfo->PThingMap->GetLayerExtents(layer).Y);
    } else {
        pos_info.PMapInfo = 0;
    }
}

}