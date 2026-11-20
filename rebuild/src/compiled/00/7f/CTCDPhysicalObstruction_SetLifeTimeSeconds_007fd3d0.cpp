// CTCDPhysicalObstruction::SetLifeTimeSeconds(float seconds) -- LifeTimer (+0x24) = (long)(ConstantFPS * seconds)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD3D0.
//   fild [0x1375550]        ; CMainGameComponent::ConstantFPS (protected static long)
//   fmul [esp+arg]          ; * seconds
//   call _ftol              ; (long) truncate
//   mov [this+0x24], eax    ; LifeTimer
// Leaf: ?SetLifeTimeSeconds@CTCDPhysicalObstruction@@QAEXM@Z
// NF123 (2026-11-20, q6401): was the stand-in CAIStateGroup_BuyFromSeller::BoughtSomething(float) over a row-defined global
//   g_BuyFromSeller_Scale. Ego_r.pdb 0x007F8680 + FableWin.pdb 0x0278A330 ?SetLifeTimeSeconds@CTCDPhysicalObstruction@@QAEXM@Z (the landed callers
//   0x00788229 / 0x0097BDA0 bind it; UpdateHover passes 60.0f on the obstruction driver); the global at 0x01375550 is
//   ?ConstantFPS@CMainGameComponent@@1JA, read through the inline public accessor GetConstantFPS(); +0x24 is the PDB member LifeTimer.
//   Ported onto engine/CTCDPhysicalObstruction.h and engine/CMainGameComponent.h (the global is no longer defined here).
#include "engine/ETCInterfaceType.h"
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CMainGameComponent.h"

void CTCDPhysicalObstruction::SetLifeTimeSeconds(float seconds)
{
    LifeTimer = (long)(CMainGameComponent::GetConstantFPS() * seconds);
}
