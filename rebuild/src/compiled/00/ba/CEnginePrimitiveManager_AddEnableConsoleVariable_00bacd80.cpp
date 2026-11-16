// CEnginePrimitiveManager::AddEnableConsoleVariable(const CCharString& name) -- empty in the retail build (`ret 4`)
// Leaf: ?AddEnableConsoleVariable@CEnginePrimitiveManager@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BACD80, 3 bytes (`ret 4`).
// Identity (NF42, round 5): Ego_r.pdb 0x00A44510 / FableWin.pdb 0x02E17550 ?AddEnableConsoleVariable@CEnginePrimitiveManager@@QAEXABVCCharString@@@Z,
// declared by engine/CEnginePrimitiveManager.h at this address. Every retail call site (15, callers.py) is a primitive-manager
// constructor passing a CCharString such as "EnableSprites" / "EnableFlareSprites" (0x00B35CD8 / 0x00B35DB1); the console
// variable registration compiles to nothing in this build. Was the placeholder `C00bacd80::M(int)`.
#include "engine/CEnginePrimitiveManager.h"

void CEnginePrimitiveManager::AddEnableConsoleVariable(const CCharString& name)
{
}
