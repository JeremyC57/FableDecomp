// NParticleEngine::CPSCEmitterGeneric::Alloc() -- the component factory: a new CPSCEmitterGeneric (operator new(0x50) 0x00BFEA1A, null-checked, then a tail jump into
//   the default constructor 0x00B103F0), returned as its CPSCBase
// Leaf: ?Alloc@CPSCEmitterGeneric@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7ED0, oracle length 24 bytes.
// Retail bytes:  push 0x50 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCEmitterGeneric::CPSCEmitterGeneric ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CPSCEmitterGeneric@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); retail references: the
//   registration table built by NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and
//   the class's DatabaseClone 0x00B15B00. The constructor is ??0CPSCEmitterGeneric@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00B103F0, whose landed row defines it.
// Naming (NF460 / NF462, queue nf453 / nf458): was the generated free function CPSCEmitterGeneric_Alloc() returning a global-namespace CPSCEmitterGeneric*, which also
//   called the constructor outside NParticleEngine. Same bytes.
// Model (NF462, ruling r460d): the shared engine/CPSCEmitterGeneric.h with its FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR + _ALLOC opt-ins (ctor, Alloc and the ten
//   CPSCBase pure-virtual overrides declared, as engine/NParticleEngine_CPSCOrbit.h does), so the class is concrete and derives from CPSCBase: no cast.
#define FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
#define FABLE_RETAIL_CPSCEMITTERGENERIC_ALLOC
#include "engine/CPSCEmitterGeneric.h"

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCEmitterGeneric::Alloc()
{
    return new CPSCEmitterGeneric;
}
}
