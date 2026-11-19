// CEngineSceneMap::AddPrimitive(CEngineInternalPrimitiveBase& prim, const C2DVector& world_pos, float size) -- file a primitive in the scene
//   map: its level from its size, its cell from the position normalised over the map area (floored, clamped to the level), the full
//   grid position stored on the primitive, then the cell of the deepest existing level (created on demand) takes it
// Leaf: ?AddPrimitive@CEngineSceneMap@@AAEXAAVCEngineInternalPrimitiveBase@@ABVC2DVector@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B93B30, 395 bytes, frame pointer, `ret 0Ch`. Callees: CEngineSceneMap::CalculateLevel 0x00B91920,
//   CEngineSceneGrid::GetCellCreate 0x00B937A0, CEngineInternalPrimitiveBase::AddToList 0x00B8FDF0, CEngineSceneGridCell::InvalidateAll
//   0x00B91710. Inline: GetTLPos / GetWidth / GetHeight, the CEngineSceneGridPos ctors and ConvertToLevel, SetGridPos,
//   GetCurrentLevels, GetGridLevel, CEngineSceneGrid::AddPrimitive and CEngineSceneGridCell::AddPrimitive.
// Retail-asm: 0x00B93BB0 `fistp dword ptr [ebp+0Ch]` and 0x00B93BEA `fistp dword ptr [ebp-4]` (each with the `fsub` / `fild ; fadd ;
//   cmp ; jne ; inc` tail): the engine inline-assembly floor, expanded twice through the shared engine/GFFloatToLongFloor.h helper.
//   FableWin calls ?GFFloatToLongFloor@@YIJM@Z twice. No asm in this source.
// Shape: FableWin.pdb 0x02E1C860 statement for statement (its two ASSERTs compiled out); Ego_d locals prim, world_pos, size,
//   level_size, map_pos, grid_pos, grid, level. level_size is unsigned (FableWin `fild qword` of (level_size, 0); retail `fild ; jge ;
//   fadd 2^32`). The `< 0` tests on the unsigned char cells are FableWin's (`movzx ; test ; jge`); retail drops them. The grid position
//   is the ConvertToLevel temporary copied back (retail 0x00B93C60..0x00B93C6A builds it at [ebp+0Dh] and copies its first word).
//   The earlier parks (DIFFER 377v395) used local models and an open-coded shift.
// Headers: CEngineSceneGridPos.h + FABLE_RETAIL_CENGINESCENEGRIDPOS_INLINE (FableWin ctor / ConvertToLevel bodies);
//   CEngineInternalPrimitiveBase.h + FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETGRIDPOS (nested in OCCLUSION);
//   CEngineSceneMap.h + FABLE_RETAIL_CENGINESCENEMAP_ADDPRIMITIVE (Grid GetCellCreate + inline private AddPrimitive, Map GetGridLevel,
//   CalculateLevel, AddPrimitive); the cell's private AddPrimitive through the existing CHECKVISIBILITYRESULTS friend; CArray At via
//   FABLE_RETAIL_CARRAY_ORIGINAL_AT_VECTOR_INDEX_PASS22. No layout change.
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