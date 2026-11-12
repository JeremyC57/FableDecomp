// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CCRCSymbolMap>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCCRCSymbolMap@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A018D0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00A018D0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00a018d0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCCRCSymbolMap@@@@SIXPAX@Z; stored by Reset 0x00A01AA0 (reloc_bindings stand-in ?Reset@CCountedSymbolMap@@QAEXPAVCCRCSymbolMap@@@Z, caller 0x00A01950); Ego_r.pdb CASoundBank::PSymbolMap (+0x4) is CCountedPointer<CCRCSymbolMap>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CCRCSymbolMap {
public:
    virtual ~CCRCSymbolMap();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CCRCSymbolMap>::DeleteData(void*);
