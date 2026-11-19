// CEngineScene::GetMapAt(const C2DVector& world_pos) -- the scene map for the cell under a world XY position, created on first use
// Leaf: ?GetMapAt@CEngineScene@@QAEAAVCEngineSceneMap@@ABVC2DVector@@@Z
// Flags: none
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CENGINESCENE_GETMAPAT
#define FABLE_RETAIL_CENGINESCENEMAP_CTOR
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/C2DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineSceneMap.h"
#include "engine/CEngineScene.h"

CEngineSceneMap& CEngineScene::GetMapAt(const C2DVector& world_pos)
{
    C2DCoordI map_coords;
    map_coords.X = GFFloatToLongFloor(world_pos.X * OneOverMapSize);
    map_coords.Y = GFFloatToLongFloor(world_pos.Y * OneOverMapSize);
    CCountedPointer<CEngineSceneMap>& map_ptr = MapList[map_coords.Y][map_coords.X];
    if (map_ptr.IsNull())
    {
        C2DBoxF area(C2DVector(map_coords.X * MapSize, map_coords.Y * MapSize), C2DExtentsF(MapSize, MapSize));
        map_ptr.Reset(new CEngineSceneMap(this, MaxMapLevels, area));
    }
    return *map_ptr;
}
