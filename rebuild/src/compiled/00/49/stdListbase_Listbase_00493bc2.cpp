// std::_List_base<const CMessageEvent*, std::allocator<const CMessageEvent*> >::_List_base(const allocator&) -- STLport list base: allocate the 12-byte sentinel node and link it to itself
// Leaf: ??0?$_List_base@PBVCMessageEvent@@V?$allocator@PBVCMessageEvent@@@std@@@std@@QAE@ABV?$allocator@PBVCMessageEvent@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493BC2: push esi ; mov esi,ecx ; and [esi],0 ; push 0Ch ; call malloc ; self-link ; mov [esi],eax ; ret 4
// (thiscall, one allocator& argument; was the __fastcall void** model ListNode_Create_CircularSentinel).
// Naming (agentNF31, 2026-11-12): DESCRIPTIVE-BY-LIBRARY. Retail is STLport 4.6 while Ego_r.pdb / FableWin.pdb are Dinkumware builds
//   (Ego_r names the equivalent same-TU COMDATs ??0/??1?$list@PBVCMessageEvent@@..@std@@ at 0x0049C87F / 0x0049CB20), so no PDB
//   carries this spelling. Evidence checked: the body is the STLport _List_base #else body of stlport/fable_containers.h; the landed
//   CTextControl::SetText 0x00492973 builds list<const CMessageEvent*> (Ego_r FindAllMessages signature 0x0049CB32) and binds this
//   symbol (RELOCATION_MATCH). T = const CMessageEvent* rests on that call site (the body is T-independent); no other template
//   arguments are invented. Repo precedent: list<long> 0x0042AB58 (FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE, explicit instantiation).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
class CMessageEvent;
template std::_List_base<const CMessageEvent*, std::allocator<const CMessageEvent*> >::_List_base(const std::allocator<const CMessageEvent*>&);
