// CWorld::GetLocalSeed() const -- the address of the main game component's LocalSeed (+0x16140), reached through the world's
// Component pointer (+0x8)
// Leaf: ?GetLocalSeed@CWorld@@QBEPAKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D800, 10 bytes: `mov eax,[ecx+8] ; add eax,16140h ; ret`. Callees: none.
// Names (agentNF250, 2026-11-22, NF248 E2): was the placeholder ?M@C0049d800@@QAEJXZ (a local `struct C0049d800 { char pad[8]; long f; }`
//   returning `f + 90432`; catalog label CVertexBufferWin32::DoSizeof, propagated over the folded body). Ego_r.pdb and FableWin.pdb
//   S_PUB32 ?GetLocalSeed@CWorld@@QBEPAKXZ; Ego_r donor order puts GetLocalSeed / GetWorldSeed at retail 0x0049D800 / 0x0049D810 (the
//   sibling landed row 0x0049D810 GetWorldSeed = Component + 0x1613C); four landed callers (0x008EE5A0 / 0x008EE600 / 0x008EE660 /
//   0x008EE6C0, CMaterialDef::GetRandom*ParticleEffect) bind this spelling for the world's local random seed. Body through FableWin.pdb's
//   inline ?GetLocalSeed@CMainGameComponent@@QBEPAKXZ (LocalSeed +0x16140, Ego_r.pdb).
// Header opt-ins (NF250, add-only): engine/CWorld.h FABLE_RETAIL_CWORLD_GETLOCALSEED (declaration), engine/CMainGameComponent.h
//   FABLE_RETAIL_CMAINGAMECOMPONENT_GETLOCALSEED (the inline accessor).
#define FABLE_RETAIL_CWORLD_GETLOCALSEED
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETLOCALSEED
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"

unsigned long* CWorld::GetLocalSeed() const
{
    return Component->GetLocalSeed();
}
