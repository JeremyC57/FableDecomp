// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEngineManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEngineManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004361E0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x004361E0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_004361e0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEngineManager@@@@SIXPAX@Z; stored by CCountedPointer<CEngineManager>::Reset 0x004366B0 (reloc_bindings, caller 0x00436FB0) and at 0x0050793D in CWorldMap::LoadWorldFromBinaryFile.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CEngineManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEngineManager>::DeleteData(void*);
