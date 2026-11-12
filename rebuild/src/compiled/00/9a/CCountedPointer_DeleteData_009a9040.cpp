// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CThreadedFile>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCThreadedFile@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9040, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009A9040`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009a9040, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCThreadedFile@@@@SIXPAX@Z; stored by CCountedPointer<CThreadedFile>::Reset 0x009A9C80 (reloc_bindings ?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z, caller 0x009D56C0); Ego_r.pdb CBankFileAsync / CRetailBank::ThreadedFile are CCountedPointer<CThreadedFile>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CThreadedFile {
public:
    virtual ~CThreadedFile();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CThreadedFile>::DeleteData(void*);
