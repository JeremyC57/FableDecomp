// CCountedPointer<NCreatureMode::CModeSitBase>::CCountedPointer(T*) -- take ownership of a new sitting mode: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@PAVCModeSitBase@NCreatureMode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094B3A0, 78 bytes, `ret 4`. Callees: operator new 0x00BFEA1A; stores DeleteData 0x00938060.
// Was landed as _unattributed::CCountedPointer_0094b3a0 (stand-in name and local count-block model).
// T (agentNF27, 2026-11-12, RELOC_CONFLICT land1011dn): the PDB name sets have exactly two CCountedPointer instantiations of the
//   sitting modes, <NCreatureMode::CModeSitBase> and <NCreatureMode::CModeSitFloor> (ctor (T*), dtor, ReleaseData, DeleteData; Reset only
//   for CModeSitBase; FableWin.pdb also the converting ctor CCountedPointer<CModeSitBase>(const CCountedPointer<CModeSitFloor>&)).
//   DeleteData 0x00938060 is the one the Reset body 0x00938070 stores (Reset<CModeSitBase> is the only Reset), and 0x0094B3A0 stores it, so
//   0x0094B3A0 / ~ 0x008AB320 (CAIStateGroup_EatAtTable::Update 0x0096CD70, a CModeSitTavernStool held as CModeSitBase) are <CModeSitBase>;
//   DeleteData 0x008AA810 is stored by 0x008AB380 and by the inline ctor in 0x0090F830 next to `new CModeSitFloor` (vtable 0x01267BBC,
//   RTTI .?AVCModeSitFloor@NCreatureMode@@), and 0x008AB380 / ~ 0x008AB3D0 serve the CModeSitFloor callers 0x0093B1D0 and 0x008A8C60, so
//   they are <CModeSitFloor>.
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT; the count block stores
//   CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#include "engine/CCountedPointer.h"
namespace NCreatureMode { class CModeSitBase; }
template CCountedPointer<NCreatureMode::CModeSitBase>::CCountedPointer(NCreatureMode::CModeSitBase*);
