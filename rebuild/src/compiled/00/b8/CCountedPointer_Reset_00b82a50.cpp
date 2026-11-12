// CCountedPointer::Reset(T* data) -- (CCountedPointer<CParticleSimulation>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCParticleSimulation@@@@QAEXPAVCParticleSimulation@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B82A50, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00B825D0.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES; was CCountedPointer_00b825d0::Reset, a stand-in with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCParticleSimulation@@@@QAEXPAVCParticleSimulation@@@Z; the body stores CCountedPointer<CParticleSimulation>::DeleteData 0x00B825D0 (FAM10, row MATCH) as the count block's DeleteFunc.
// Shape: engine/CCountedPointer.h Reset: ReleaseData expanded (its trailing Data store is dead), Data = data, then the count block
//   for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
class CParticleSimulation;
template void CCountedPointer<CParticleSimulation>::Reset(CParticleSimulation*);
