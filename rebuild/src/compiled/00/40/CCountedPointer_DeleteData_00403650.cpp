// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NGameText::CDataBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCDataBank@NGameText@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403650, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00403650`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00403650, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCDataBank@NGameText@@@@SIXPAX@Z; stored by CCountedPointer<NGameText::CDataBank>::Reset 0x00403E40 (reloc_bindings, caller 0x00416832); Ego_r.pdb CMainGameComponent::PTextBank is CCountedPointer<NGameText::CDataBank>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/NGameText_CDataBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NGameText::CDataBank>::DeleteData(void*);
