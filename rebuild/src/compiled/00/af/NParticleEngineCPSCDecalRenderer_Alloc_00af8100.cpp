// NParticleEngine::CPSCDecalRenderer::Alloc() -- the particle-component factory: a new CPSCDecalRenderer (operator new(0x64) 0x00BFEA1A,
//   null-checked, then a tail jump into the out-of-line default constructor 0x00B1DFD0), returned as its CPSCBase
// Leaf: ?Alloc@CPSCDecalRenderer@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8100, oracle length 24 bytes.
// Retail bytes:  push 0x64 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCDecalRenderer::CPSCDecalRenderer ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb + FableWin.pdb ?Alloc@CPSCDecalRenderer@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); its retail references are
//   the registry CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and DatabaseClone 0x00B1FF80 (respelt
//   in this unit). The constructor is ??0CPSCDecalRenderer@NParticleEngine@@QAE@XZ (Ego_r 0x00ADE5E0 + FableWin 0x02F55400), 0x00B1DFD0 (its
//   only retail reference is this tail jump), whose landed row defines it.
// Naming (NF463; the NF462 r460d / bB2 pattern): was the generated free function `CPSCDecalRenderer* CPSCDecalRenderer_Alloc()` (cdecl, no
//   arguments: the same code) over a flat 0x64-byte struct, calling the constructor as the global-namespace ??0CPSCDecalRenderer@@QAE@XZ; same bytes.
// Model: the shared engine/CPSCDecalRenderer.h (sizeof 0x64 asserted there) with its ctor opt-in FABLE_RETAIL_CPSCDECALRENDERER_CTOR and the new
//   add-only FABLE_RETAIL_CPSCDECALRENDERER_ALLOC (Alloc + the CPSCBase pure overrides it did not declare; declarations only). No cast.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CPSCDECALRENDERER_CTOR
#define FABLE_RETAIL_CPSCDECALRENDERER_ALLOC
#include "engine/CPSCDecalRenderer.h"

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCDecalRenderer::Alloc()
{
    return new CPSCDecalRenderer;
}
}
