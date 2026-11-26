// NParticleEngine::CPSCUpdateNormal::Alloc() -- the component factory: a new CPSCUpdateNormal (operator new(0xa0) 0x00BFEA1A, null-checked, then a tail jump into
//   the default constructor 0x00B0C9C0), returned as its CPSCBase
// Leaf: ?Alloc@CPSCUpdateNormal@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7E60, oracle length 24 bytes.
// Retail bytes:  push 0xa0 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCUpdateNormal::CPSCUpdateNormal ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CPSCUpdateNormal@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); retail references: the
//   registration table built by NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and
//   the class's DatabaseClone 0x00B0F970. The constructor is ??0CPSCUpdateNormal@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00B0C9C0, whose landed row defines it.
// Naming (NF460 / NF462, queue nf453 / nf458): was the generated free function CPSCUpdateNormal_Alloc() returning a global-namespace CPSCUpdateNormal*, which also
//   called the constructor outside NParticleEngine. Same bytes.
// Model (NF462, ruling r460d): the shared engine/CPSCUpdateNormal.h on its containers-free view (FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS: no /DEFAULTLIB:"libcp", r450a) with its FABLE_RETAIL_CPSCUPDATENORMAL_ALLOC opt-in (ctor, Alloc and the ten
//   CPSCBase pure-virtual overrides declared, as engine/NParticleEngine_CPSCOrbit.h does), so the class is concrete and derives from CPSCBase: no cast.
#define FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS
#define FABLE_RETAIL_CPSCUPDATENORMAL_ALLOC
#include "engine/CPSCUpdateNormal.h"

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCUpdateNormal::Alloc()
{
    return new CPSCUpdateNormal;
}
}
