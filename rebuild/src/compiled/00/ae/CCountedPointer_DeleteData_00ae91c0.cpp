// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NParticleEngine::CParticleDataBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCParticleDataBank@NParticleEngine@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE91C0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00AE91C0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00ae91c0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCParticleDataBank@NParticleEngine@@@@SIXPAX@Z; stored by CCountedPointer<NParticleEngine::CParticleDataBank>::Reset 0x00AE93E0 (reloc_bindings, caller 0x00AE8980); Ego_r.pdb CParticleEmitterDatabase::EmitterBank is CCountedPointer<NParticleEngine::CParticleDataBank>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
namespace NParticleEngine {
class CParticleDataBank {
public:
    virtual ~CParticleDataBank();                                   // slot 0 (scalar deleting destructor)
};
}

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NParticleEngine::CParticleDataBank>::DeleteData(void*);
