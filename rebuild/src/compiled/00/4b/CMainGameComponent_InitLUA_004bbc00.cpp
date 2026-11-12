// CMainGameComponent::InitLUA(const CCharString& script) -- the LUA start-up hook PostInit calls with GRunLUAScript; empty in the retail build
// Leaf: ?InitLUA@CMainGameComponent@@IAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BBC00, 3 bytes (`ret 4`).
// Name (agentNF26, 2026-11-12, found while respelling FAM09's GRunLUAScript binder 0x00416953): Ego_r.pdb + FableWin.pdb protected
//   ?InitLUA@CMainGameComponent@@IAEXABVCCharString@@@Z. Ego_r's InitLUA (0x004BF6D0) is the same one-instruction 3-byte body (strict
//   signature hash 57b80449b3ee4b71 = retail 0x004BBC00, ghidra_out/naming_stragglers), and its only caller, CMainGameComponent::PostInit
//   0x00416953, passes GRunLUAScript (CCharString 0x013B8674) with `this` in ecx. Was a stand-in C004bbc00::M(int) (catalog
//   _global::Nop_ret4_004bbc00, earlier $E2); the caller spelled it CMainGameComponent::Register(CRegistration*).
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_INITLUA   // hdr overlay (agentNF26)
#include "engine/CMainGameComponent.h"

void CMainGameComponent::InitLUA(const CCharString& script)
{
}
