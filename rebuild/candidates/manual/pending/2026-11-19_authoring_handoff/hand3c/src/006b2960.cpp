// CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const
// Leaf: ?GetNavigationInformation@CGameNavigatorManagerExternalInterface@@UBEXABVC2DBoxF@@AAVCNavigationAreaUpdate@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2960, 250 bytes, `ret 8`, frame pointer. Retail vtable 0x01238C58 slot 1. Callees:
//   World->PeekWorldMap() (CWorld virtual slot 13, +0x34, twice), CWorldMap::GetMapNumberAt(const C2DCoordI&) (slot 17, +0x44),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, and CWorldMap::GetMapNavigationAreaUpdateInfo (0x00509A20).
// Retail-asm: 0x006B2973 `fld dword ptr [ebp+8]` + 0x006B2976 `fistp dword ptr [ebp-1Ch]` (round) and 0x006B299C `fsub dword ptr [ebp-8]` +
//   0x006B299F `fistp dword ptr [ebp-4]` (floor, then `fild ; fadd ; cmp ; inc`): the engine inline-assembly float-to-long helpers,
//   expanded through the shared engine/GFFloatToLongNear.h and engine/GFFloatToLongFloor.h. FableWin 0x02461570 calls
//   ?GFFloatToLongNear@@YIJM@Z and ?GFFloatToLongFloor@@YIJM@Z by name. No asm in this source.
// Shape: FableWin.pdb 0x02461570 without its debug ASSERT blocks (GetMapBox bounds checks, compiled out). In order:
//   C2DBoxI(Floor TLX, Floor TLY, Near BRX, Near BRY) (??0C2DBoxI@@QAE@JJJJ@Z); GetMapNumberAt(box.GetTLPos()); then
//   GetMapNavigationAreaUpdateInfo(map, box, CCharString("NULL"), update). The "NULL" text is at 0x012393E4 in retail and 0x0421742C
//   in FableWin. Retail builds the string before it evaluates the other arguments, and keeps it in the dead `box` parameter slot,
//   which it shares with the helpers' temporaries. A block-scoped named local gives exactly that. A full-expression temporary
//   pushes `update` first (DIFFER 249v250). A function-scope local takes a slot of its own (DIFFER 250v250, frame 0x34).
//   Local names are descriptive (no Ego_d record).
// Inferred: the block-scoped named `name` local. FableWin (debug) shows only a CCharString temporary at this call, so the block and
//   the local are the byte-required spelling, not a sourced one. Measured alternatives: a full-expression temporary gives DIFFER 249v250
//   (it loads `update` and World before the ctor); a function-scope named local gives DIFFER 250v250 (frame 0x34).
// Headers: CGameNavigatorManagerExternalInterface.h gains FABLE_RETAIL_CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION. Ego_r
//   field list 0x32FC4 makes slot 1 GetNavigationInformation, pure, where the default path has the placeholder `Initialise()`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION
#include "fable_string.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DBoxI.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CGameNavigatorManagerExternalInterface.h"

void CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const
{
    C2DBoxI box_in_cells(GFFloatToLongFloor(box.TLX), GFFloatToLongFloor(box.TLY), GFFloatToLongNear(box.BRX), GFFloatToLongNear(box.BRY));
    long map_number = World->PeekWorldMap().GetMapNumberAt(box_in_cells.GetTLPos());
    {
        CCharString name("NULL");
        World->PeekWorldMap().GetMapNavigationAreaUpdateInfo(map_number, box_in_cells, name, update);
    }
}
