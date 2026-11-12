// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CParticleSimulation>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCParticleSimulation@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B825D0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B825D0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b825d0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCParticleSimulation@@@@SIXPAX@Z; stored by Reset 0x00B82A50 (reloc_bindings stand-in ?Reset@CParticleSimPointer@@QAEXPAVCParticleSimulation@@@Z, caller 0x00B82100), called at 0x00B82048 / 0x00B82258 with a fresh CParticleSimulation (ctor 0x00AE0600, RTTI vtable).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CParticleSimulation.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CParticleSimulation>::DeleteData(void*);
