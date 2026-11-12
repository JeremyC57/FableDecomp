// CCountedPointer<NCreatureMode::CModeSitFloor>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCModeSitFloor@NCreatureMode@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB3D0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as CCountedPointer<NCreatureMode::CModeSitBase>::~CCountedPointer (T read from the 0x0093B1D0 binding, which held the floor
//   mode in a CCountedPointer<CModeSitBase>; that symbol is 0x008AB320).
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T).
// T (agentNF27, 2026-11-12, RELOC_CONFLICT land1011dn): the PDB name sets have exactly two CCountedPointer instantiations of the
//   sitting modes, <NCreatureMode::CModeSitBase> and <NCreatureMode::CModeSitFloor> (ctor (T*), dtor, ReleaseData, DeleteData; Reset only
//   for CModeSitBase; FableWin.pdb also the converting ctor CCountedPointer<CModeSitBase>(const CCountedPointer<CModeSitFloor>&)).
//   DeleteData 0x00938060 is the one the Reset body 0x00938070 stores (Reset<CModeSitBase> is the only Reset), and 0x0094B3A0 stores it, so
//   0x0094B3A0 / ~ 0x008AB320 (CAIStateGroup_EatAtTable::Update 0x0096CD70, a CModeSitTavernStool held as CModeSitBase) are <CModeSitBase>;
//   DeleteData 0x008AA810 is stored by 0x008AB380 and by the inline ctor in 0x0090F830 next to `new CModeSitFloor` (vtable 0x01267BBC,
//   RTTI .?AVCModeSitFloor@NCreatureMode@@), and 0x008AB380 / ~ 0x008AB3D0 serve the CModeSitFloor callers 0x0093B1D0 and 0x008A8C60, so
//   they are <CModeSitFloor>.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
namespace NCreatureMode { class CModeSitFloor; }
template CCountedPointer<NCreatureMode::CModeSitFloor>::~CCountedPointer();
