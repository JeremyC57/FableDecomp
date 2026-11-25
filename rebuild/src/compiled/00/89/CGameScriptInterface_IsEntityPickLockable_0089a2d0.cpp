// CGameScriptInterface::IsEntityPickLockable(const CScriptThing& entity) const -- the live thing's door component, if any, is lock-pickable
// Leaf: ?IsEntityPickLockable@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089A2D0. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.IsAlive() (CScriptThing slot 75, +0x12c), entity.GetPThing() (slot 11, +0x2c), then
//   ??$PeekTC@$$CBVCTCDoor@@@CThing@@QBE_NPAPBVCTCDoor@@@Z (inline in retail: the UsingComponents bit 0x26 TCI_DOOR, the const
//   CVectorMap::LowerBound 0x004365B0, the end-or-greater test) and ?IsLockPickable@CTCDoor@@QBE_NXZ (retail 0x00730AE0, the call target).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCDOOR
#define FABLE_RETAIL_CTCDOOR_VANDALISM
#define FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/CTCDoor.h"

bool CGameScriptInterface::IsEntityPickLockable(const CScriptThing& entity) const
{
    if (entity.IsAlive()) {
        const CThing* thing = entity.GetPThing();
        const CTCDoor* door;
        if (thing->PeekTC(&door) && door->IsLockPickable())
            return true;
    }
    return false;
}
