// NParticleEngine::CPSCOrbit::Alloc() -- the component factory: a new CPSCOrbit (operator new(0x174) 0x00BFEA1A, null-checked, then a tail jump into
//   the default constructor 0x00B16EB0), returned as its CPSCBase
// Leaf: ?Alloc@CPSCOrbit@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7F40, oracle length 27 bytes.
// Retail bytes:  push 0x174 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCOrbit::CPSCOrbit ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CPSCOrbit@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); retail references: the
//   registration table built by NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170 (whose landed row already binds this spelling here) and
//   the class's DatabaseClone. The constructor is ??0CPSCOrbit@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00B16EB0, whose landed row defines it.
// Naming (NF460, queue nf453 / nf458): was the generated free function CPSCOrbit_Alloc() returning a global-namespace CPSCOrbit*, which also
//   called the constructor outside NParticleEngine. Same bytes.
#include "engine/NParticleEngine_CPSCOrbit.h"

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCOrbit::Alloc()
{
    return new CPSCOrbit;
}
}
