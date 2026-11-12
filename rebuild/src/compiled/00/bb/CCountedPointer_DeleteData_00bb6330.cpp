// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CParticleSimulationRenderData>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCParticleSimulationRenderData@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB6330, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00BB6330`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00bb6330, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCParticleSimulationRenderData@@@@SIXPAX@Z; stored by Reset 0x00BB64A0, called at 0x00BB260A with a fresh CEngineParticleSimulationRenderData (ctor 0x00BB63A0, RTTI) ; Ego_r.pdb CParticleSimulation::RenderData (+0x4) is CCountedPointer<CParticleSimulationRenderData>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CPARTICLESIMULATIONRENDERDATA_VIRTUAL_DTOR
#include "engine/CParticleSimulationRenderData.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CParticleSimulationRenderData>::DeleteData(void*);
