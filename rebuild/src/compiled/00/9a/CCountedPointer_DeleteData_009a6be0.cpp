// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CGraphicBankManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCGraphicBankManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A6BE0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009A6BE0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009a6be0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCGraphicBankManager@@@@SIXPAX@Z; stored by Reset 0x009A71F0 (reloc_bindings stand-in ?Reset@CPGraphic@@QAEXPAVCGraphicBankManager@@@Z), called at 0x009A6940 with a fresh CGraphicBankManager (ctor 0x009F8690, RTTI vtable); Ego_r.pdb CSystemManager::PGraphicBankManager is CCountedPointer<CGraphicBankManager>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CGraphicBankManager {
public:
    virtual ~CGraphicBankManager();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CGraphicBankManager>::DeleteData(void*);
