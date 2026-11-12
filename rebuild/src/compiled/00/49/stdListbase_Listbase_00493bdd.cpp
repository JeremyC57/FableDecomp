// std::_List_base<const CMessageEvent*, std::allocator<const CMessageEvent*> >::~_List_base() -- STLport list base: clear() (0x00493911, out of line) then free the sentinel node
// Leaf: ??1?$_List_base@PBVCMessageEvent@@V?$allocator@PBVCMessageEvent@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493BDD: push esi ; mov esi,ecx ; call 0x00493911 ; mov esi,[esi] ; test esi,esi ; je ; push esi ;
// call free ; pop ecx ; pop esi ; ret (was the placeholder std::list_T_00493bdd::~list_T_00493bdd).
// Naming (agentNF31, 2026-11-12): DESCRIPTIVE-BY-LIBRARY. Retail is STLport 4.6 while Ego_r.pdb / FableWin.pdb are Dinkumware builds
//   (Ego_r names the equivalent same-TU COMDATs ??0/??1?$list@PBVCMessageEvent@@..@std@@ at 0x0049C87F / 0x0049CB20), so no PDB
//   carries this spelling. Evidence checked: the body is the STLport _List_base #else body of stlport/fable_containers.h; the landed
//   CTextControl::SetText 0x00492973 builds list<const CMessageEvent*> (Ego_r FindAllMessages signature 0x0049CB32) and binds this
//   symbol (RELOCATION_MATCH). T = const CMessageEvent* rests on that call site (the body is T-independent); no other template
//   arguments are invented. Repo precedent: list<long> 0x0042AB58 (FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE, explicit instantiation).
// The clear 0x00493911 is declared only (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE): it binds ?clear@?$_List_base@PBVCMessageEvent@@..@std@@QAEXXZ
//   (the same instantiation; the 0x00493911 row is still the placeholder _global::StdList_Clear_00493911).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
class CMessageEvent;
template std::_List_base<const CMessageEvent*, std::allocator<const CMessageEvent*> >::~_List_base();
