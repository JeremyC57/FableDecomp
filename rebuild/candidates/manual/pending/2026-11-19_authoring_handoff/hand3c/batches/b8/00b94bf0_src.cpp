// CEngineScene::GetMapAt(const C2DVector& world_pos) -- the scene map for the cell under a world XY position, created on first use
// Leaf: ?GetMapAt@CEngineScene@@QAEAAVCEngineSceneMap@@ABVC2DVector@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B94BF0, 309 bytes, frame pointer, `ret 4`. Callees: the outer map<long, map<long,
//   CCountedPointer<CEngineSceneMap> > >::operator[] 0x00B988E0 and the inner map<long, CCountedPointer<CEngineSceneMap> >::operator[]
//   0x00B97C10 (both out of line, landed outer), operator new 0x00BFEA1A (0x28 bytes), CBaseClassNonCopyable::CBaseClassNonCopyable
//   0x0099A2F0, CEngineSceneMap::Initialise 0x00B949A0 and CCountedPointer<CEngineSceneMap>::Reset 0x00B95AA0. Vtable 0x012A3E60.
// Retail-asm: 0x00B94C16 `fsub dword ptr [ebp-8]` + 0x00B94C19 `fistp dword ptr [ebp+8]` and 0x00B94C50 `fsub` + 0x00B94C53 `fistp dword ptr
//   [ebp-4]` (each followed by `fild ; fadd ; cmp ; inc`): the engine inline-assembly floor, expanded twice through the shared
//   engine/GFFloatToLongFloor.h helper. FableWin calls ?GFFloatToLongFloor@@YIJM@Z twice. No asm in this source.
// Shape: FableWin.pdb 0x02E1D7D0. map_coords = (Floor(X * OneOverMapSize), Floor(Y * OneOverMapSize)); map_ptr = MapList[Y][X];
//   if (map_ptr.IsNull()) the area C2DBoxF(C2DVector(X * MapSize, Y * MapSize), C2DExtentsF(MapSize, MapSize)) and
//   Reset(new CEngineSceneMap(this, MaxMapLevels, area)); return *map_ptr. Ego_d names: world_pos, map_ptr, map_coords, area.
//   FABLE_RETAIL_STLP_MAP keeps the subscripts out of line with their STLport body visible. VC7.1 then knows the key is only read:
//   it builds the area from the floor results, stores the Y key right before the call, and does not reload map_coords (without it,
//   DIFFER 311v311 in the frame slots).
// Headers: CEngineScene.h + FABLE_RETAIL_CENGINESCENE_GETMAPAT (typed MapList, GetMapAt); CEngineSceneMap.h +
//   FABLE_RETAIL_CENGINESCENEMAP_CTOR (CBaseClassNonCopyable base per RTTI, the inline ctor, Initialise). No layout change.
//   The earlier park (IMPROVED 297v311) used a local model.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_MAP
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
