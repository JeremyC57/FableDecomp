// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NTCScriptedControl::CScriptConversation>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCScriptConversation@NTCScriptedControl@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00718080, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00718080`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00718080, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCScriptConversation@NTCScriptedControl@@@@SIXPAX@Z; stored by CCountedPointer<NTCScriptedControl::CScriptConversation>::Reset 0x00718940 (reloc_bindings, caller 0x00714D30); Ego_r.pdb CActionTalkBase::Conversation is CCountedPointer<NTCScriptedControl::CScriptConversation>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
namespace NTCScriptedControl {
class CScriptConversation {
public:
    virtual ~CScriptConversation();                                   // slot 0 (scalar deleting destructor)
};
}

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<NTCScriptedControl::CScriptConversation>::DeleteData(void*);
