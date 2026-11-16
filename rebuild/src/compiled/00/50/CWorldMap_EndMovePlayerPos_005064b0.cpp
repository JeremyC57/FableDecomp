// CWorldMap::EndMovePlayerPos() -- finish a player move: cache the maps, waiting for the load (CacheMaps(WAIT_FOR_LOAD))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005064B0, oracle length 8 bytes.
// Leaf: ?EndMovePlayerPos@CWorldMap@@QAEXXZ
// Retail bytes:  push 0 ; call 0x502850 ; ret
// Name fix (agentNF36 2026-11-16): was the stand-in CClass_005064b0::CallWith_005064b0. Ego_r.pdb + FableWin.pdb name sets hold
// ?EndMovePlayerPos@CWorldMap@@QAEXXZ; its landed callers 0x004A3740 (CWorld::UpdateRegionLoad) and 0x0089B780 bind it. The callee
// 0x00502850 is ?CacheMaps@CWorldMap@@IAEXW4ELoadType@1@@Z (both PDBs; nested ELoadType, WAIT_FOR_LOAD = 0).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_MOVEPLAYERPOS
#define FABLE_RETAIL_CWORLDMAP_CACHEMAPS
#include "engine/CWorldMap.h"

void CWorldMap::EndMovePlayerPos()
{
    CacheMaps(WAIT_FOR_LOAD);
}
