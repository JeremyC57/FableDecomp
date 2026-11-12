// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CFileInstaller>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCFileInstaller@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403630, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00403630`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00403630, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCFileInstaller@@@@SIXPAX@Z; stored by CCountedPointer<CFileInstaller>::Reset 0x00403DB0 (reloc_bindings ?Reset@?$CCountedPointer@VCFileInstaller@@@@QAEXPAVCFileInstaller@@@Z, caller 0x00404440).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CFileInstaller.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CFileInstaller>::DeleteData(void*);
