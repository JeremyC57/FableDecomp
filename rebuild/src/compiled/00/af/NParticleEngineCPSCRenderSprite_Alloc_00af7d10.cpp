// NParticleEngine::CPSCRenderSprite::Alloc() -- the component factory: a new CPSCRenderSprite (operator new(0x54) 0x00BFEA1A, null-checked, then a tail jump into
//   the default constructor 0x00AFB7C0), returned as its CPSCBase
// Leaf: ?Alloc@CPSCRenderSprite@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7D10, oracle length 24 bytes.
// Retail bytes:  push 0x54 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCRenderSprite::CPSCRenderSprite ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CPSCRenderSprite@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); retail references: the
//   registration table built by NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and
//   the class's DatabaseClone 0x00AFE580. The constructor is ??0CPSCRenderSprite@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00AFB7C0, whose landed row defines it.
// Naming (NF460 / NF462, queue nf453 / nf458): was the generated free function CPSCRenderSprite_Alloc() returning a global-namespace CPSCRenderSprite*, which also
//   called the constructor outside NParticleEngine. Same bytes.
// Model (NF462, ruling r460d): the shared engine/CPSCRenderSprite.h with its FABLE_RETAIL_CPSCRENDERSPRITE_ALLOC opt-in (ctor, Alloc and the ten
//   CPSCBase pure-virtual overrides declared, as engine/NParticleEngine_CPSCOrbit.h does), so the class is concrete and derives from CPSCBase: no cast.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CPSCRENDERSPRITE_ALLOC
#include "engine/CPSCRenderSprite.h"

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCRenderSprite::Alloc()
{
    return new CPSCRenderSprite;
}
}
