// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CCodeSectionManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCCodeSectionManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00413300, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00413300`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00413300, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCCodeSectionManager@@@@SIXPAX@Z; stored by the landed CCountedPointer<CCodeSectionManager>::Reset 0x00413590 (leaf ?Reset@?$CCountedPointer@VCCodeSectionManager@@@@QAEXPAVCCodeSectionManager@@@Z, first caller 0x009BCF70).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CCodeSectionManager {
public:
    virtual ~CCodeSectionManager();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CCodeSectionManager>::DeleteData(void*);
