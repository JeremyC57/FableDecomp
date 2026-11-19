// CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& prim, const C2DVector& world_pos, float size) -- file a primitive in the scene map
// Leaf: ?AddPrimitive@CEngineSceneMap@@AAEXAAVCEngineInternalPrimitiveBase@@ABVC2DVector@@M@Z
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETGRIDPOS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA
#define FABLE_RETAIL_CENGINESCENEGRIDCELL_CHECKVISIBILITYRESULTS
#define FABLE_RETAIL_CENGINESCENEGRIDPOS_INLINE
#define FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE
#define FABLE_RETAIL_CARRAY_ORIGINAL_AT_VECTOR_INDEX_PASS22
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERPOS
#include "engine/GFFloatToLongFloor.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CEngineSceneGridPos.h"
#include "engine/CEngineSceneGridCell.h"
#include "engine/CEngineSceneMap.h"

void CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& prim, const C2DVector& world_pos, float size)
{
    C2DVector map_pos = world_pos - Area.GetTLPos();
    map_pos.X /= Area.GetWidth();
    map_pos.Y /= Area.GetHeight();
    CEngineSceneGridPos grid_pos;
    grid_pos.Level = (signed char)CalculateLevel(size);
    unsigned long level_size = 1 << grid_pos.Level;
    grid_pos.X = (unsigned char)GFFloatToLongFloor(level_size * map_pos.X);
    grid_pos.Y = (unsigned char)GFFloatToLongFloor(level_size * map_pos.Y);
    if (grid_pos.X < 0)
        grid_pos.X = 0;
    else if (grid_pos.X > level_size - 1)
        grid_pos.X = (unsigned char)(level_size - 1);
    if (grid_pos.Y < 0)
        grid_pos.Y = 0;
    else if (grid_pos.Y > level_size - 1)
        grid_pos.Y = (unsigned char)(level_size - 1);
    prim.SetGridPos(grid_pos);
    unsigned long level = (unsigned long)grid_pos.Level < GetCurrentLevels() - 1 ? grid_pos.Level : GetCurrentLevels() - 1;
    grid_pos = grid_pos.ConvertToLevel(level);
    CEngineSceneGrid* grid = GetGridLevel(level);
    grid->AddPrimitive(grid_pos.X, grid_pos.Y, prim);
}
