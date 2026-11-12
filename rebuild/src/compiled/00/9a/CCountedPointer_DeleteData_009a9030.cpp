// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CDiskFileWin32>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9030, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009A9030`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009a9030, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@SIXPAX@Z; stored by the CCountedPointer<CDiskFileWin32>(T*) ctor 0x009A9B60 and Reset 0x009D1EE0 (reloc_bindings, caller 0x009A8840); Ego_r.pdb CBankFile::BankFile / CRetailBank::DiskFile.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CDISKFILEWIN32
#include "engine/CDiskFileWin32.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CDiskFileWin32>::DeleteData(void*);
