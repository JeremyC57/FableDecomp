// NParticleEngine::CPSCLight::Alloc() -- the component factory: a new CPSCLight (operator new(0x6c) 0x00BFEA1A, null-checked, then a tail jump into
//   the default constructor 0x00B1A640), returned as its CPSCBase
// Leaf: ?Alloc@CPSCLight@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8020, oracle length 24 bytes.
// Retail bytes:  push 0x6c ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCLight::CPSCLight ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CPSCLight@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); retail references: the
//   registration table built by NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and
//   the class's DatabaseClone 0x00B1AE70. The constructor is ??0CPSCLight@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00B1A640, whose landed row defines it.
// Naming (NF460 / NF462, queue nf453 / nf458): was the generated free function CPSCLight_Alloc() returning a global-namespace CPSCLight*, which also
//   called the constructor outside NParticleEngine. Same bytes.
// Model (NF462, ruling r460d): the shared engine/CPSCLight.h on its containers-free view (FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS: no /DEFAULTLIB:"libcp", r450a) with its FABLE_RETAIL_CPSCLIGHT_ALLOC opt-in (ctor, Alloc and the ten
//   CPSCBase pure-virtual overrides declared, as engine/NParticleEngine_CPSCOrbit.h does), so the class is concrete and derives from CPSCBase: no cast.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS
#define FABLE_RETAIL_CPSCLIGHT_ALLOC
#include "engine/CPSCLight.h"

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCLight::Alloc()
{
    return new CPSCLight;
}
}
