// CEngineSceneMap::Update() -- resize the map's grid levels to the scene's primitive density: side = round(sqrt(total primitives / max average per cell)), levels = top bit of (2 side - 1) + 1, limited to 1..MaxLevels
// Leaf: ?Update@CEngineSceneMap@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B947E0, 246 bytes, `ret`. Ego_r.pdb ?Update@CEngineSceneMap@@QAEXXZ.
// Callees: CEngineSceneGrid::GetCellTotalPrimitiveCount(0, 0) 0x00B91D20, CEngineSceneMap::ResizeLevels(long) 0x00B94540.
//   Inline: CEngineScene::GetMaxAveragePrimitivesPerCell, GFGetTopBit (FableWin.pdb ?GFGetTopBit@@YIKK@Z), GFLimit, GetCurrentLevels.
// Ego_d locals: total_count (unsigned long), new_size (long), new_levels (unsigned long).
// Matched (hand2c 2026-11-19): new engine/CEngineSceneMap.h (Ego_d class record) + CEngineScene accessor opt-in, the shared
//   GFFloatToLongNear helper, FableWin's GFGetTopBit expanded (existing opt-in FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE: retail expands
//   it, 0x00B9482D..0x00B948B0) on `new_size > 1 ? new_size : 1` (the clamp as a statement keeps new_size and the TopBit copy in two
//   registers), and the PDB GFLimit template for 1..MaxLevels.
// Retail-asm: 0x00B94813 `fld dword ptr [esp+8]` + 0x00B94817 `fistp dword ptr [esp+0Ch]` -- the engine's inline-assembly
//   GFFloatToLongNear (shared engine/GFFloatToLongNear.h; the float copied to its parameter slot then a bare dword fistp). VC7.1 emits
//   no bare dword fistp from C++ (`(long)x` = __ftol2, /QIfist = `fistp qword`; per-site attempt table in the helper header's
//   evidence block, 0x00BA115C/ROUNDING_ATTEMPTS.md). This row carries no asm of its own.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE
#define FABLE_RETAIL_CENGINESCENE_GETMAXAVERAGEPRIMITIVESPERCELL
#include <math.h>
#pragma intrinsic(sqrt)
#include "fable_string.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFGetNextPowerOfTwo.h"
#include "engine/GFLimit.h"
#include "engine/CEngineScene.h"
#include "engine/CEngineSceneMap.h"

void CEngineSceneMap::Update()
{
    unsigned long total_count = GridLevels[0]->GetCellTotalPrimitiveCount(0, 0);
    long new_size = GFFloatToLongNear((float)sqrt(total_count / Scene->GetMaxAveragePrimitivesPerCell())) * 2 - 1;
    unsigned long new_levels = GFGetTopBit(new_size > 1 ? new_size : 1) + 1;
    GFLimit(new_levels, 1UL, MaxLevels);
    if (new_levels != GetCurrentLevels())
        ResizeLevels(new_levels);
}
