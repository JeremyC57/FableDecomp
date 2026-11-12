// CCountedPointer::Reset(T* data) -- (CCountedPointer<NCreatureMode::CModeSitBase>) drop the current share (destroying the mode on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAEXPAVCModeSitBase@NCreatureMode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00938070, 103 bytes, thiscall, `ret 4`. Callees: the old block's DeleteFunc (indirect), operator delete
//   0x00BFE9BC, operator new 0x00BFEA1A; stores DeleteData 0x00938060.
// Names (agentNF27, 2026-11-12; was CCountedPointer_00938060::Reset with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAEXPAVCModeSitBase@NCreatureMode@@@Z is the only Reset of a sitting-mode
//   pointer (the Anniv Test PDB has the same Reset / DeleteData pair); the deleter it stores, 0x00938060, is also stored by the (T*) ctor 0x0094B3A0, which
//   CAIStateGroup_EatAtTable::Update 0x0096CD70 uses for a CModeSitTavernStool held as CCountedPointer<CModeSitBase> (no
//   CCountedPointer<CModeSitTavernStool> exists in either PDB). It sits in the 0x009380E0..0x00938280 sitting-mode block
//   (CModeSitChairBored / CModeSitChairSleeping ctors, CAIStateGroup_UsesSeat::AllocChairSittingMode).
// Shape: engine/CCountedPointer.h Reset: ReleaseData expanded (its trailing Data store is dead), Data = data, then the count block
//   for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
namespace NCreatureMode { class CModeSitBase; }
template void CCountedPointer<NCreatureMode::CModeSitBase>::Reset(NCreatureMode::CModeSitBase*);
