// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CConsole>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCConsole@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004132F0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x004132F0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_004132f0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCConsole@@@@SIXPAX@Z; stored by CCountedPointer<CConsole>::Reset 0x00413520 (reloc_bindings ?Reset@?$CCountedPointer@VCConsole@@@@QAEXPAVCConsole@@@Z, first caller 0x00414C90).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CConsole {
public:
    virtual ~CConsole();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CConsole>::DeleteData(void*);
