// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NCreatureMode::CModeSitFloor>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the mode's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCModeSitFloor@NCreatureMode@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA810, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member
//   (data in ecx); only its address is used: it is stored as the count block's DeleteFunc.
// Names: was _global::DeleteViaVtable0_008aa810 (Destroy_008aa810 in 0x008AB380, DeleteSitMode in 0x0090F830).
// T (agentNF27, 2026-11-12, RELOC_CONFLICT land1011dn): the PDB name sets have exactly two CCountedPointer instantiations of the
//   sitting modes, <NCreatureMode::CModeSitBase> and <NCreatureMode::CModeSitFloor>, each with its own DeleteData (FableWin.pdb
//   ?DeleteData@?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@SIXPAX@Z / ...VCModeSitFloor...; Anniv Test PDB the same pair).
//   0x00938060 is stored by the Reset body 0x00938070 (Reset<CModeSitBase> is the only Reset in either PDB) and by the CModeSitBase ctor
//   0x0094B3A0 (CAIStateGroup_EatAtTable::Update, a CModeSitTavernStool held as CModeSitBase); 0x008AA810 is stored by the
//   CModeSitFloor ctor 0x008AB380 (callers 0x0093B1D0 / 0x008A8C60 build a CModeSitFloor) and by the inline ctor in 0x0090F830 next to
//   `new CModeSitFloor` (vtable 0x01267BBC, RTTI .?AVCModeSitFloor@NCreatureMode@@).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T (CModeBase has a virtual dtor, slot 0):
//   `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]` (the scalar deleting destructor); the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

template void FABLE_FASTCALL CCountedPointer<NCreatureMode::CModeSitFloor>::DeleteData(void*);
