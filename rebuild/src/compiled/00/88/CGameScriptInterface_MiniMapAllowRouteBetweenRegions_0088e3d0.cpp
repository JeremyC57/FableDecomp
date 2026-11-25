// CGameScriptInterface::MiniMapAllowRouteBetweenRegions(const CCharString& region_1, const CCharString& region_2, bool allow) const -- the world map's SetRegionsAsConnectedInGraph
// Leaf: ?MiniMapAllowRouteBetweenRegions@CGameScriptInterface@@UBEXABVCCharString@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E3D0. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: World (+0x04), the virtual ?GetWorldMap@CWorld@@UAEAAVCWorldMap@@XZ (slot 12, +0x30), then
//   ?SetRegionsAsConnectedInGraph@CWorldMap@@QAEXABVCCharString@@0_N@Z (retail 0x004FC970), arguments in order.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_SETREGIONSASCONNECTED
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"

void CGameScriptInterface::MiniMapAllowRouteBetweenRegions(const CCharString& region_1, const CCharString& region_2, bool allow) const
{
    World->GetWorldMap().SetRegionsAsConnectedInGraph(region_1, region_2, allow);
}
