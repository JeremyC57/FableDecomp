// std::_List_base<CWideString, std::allocator<CWideString> >::~_List_base() -- STLport list base destructor: clear the nodes (out-of-line _List_base::clear @ 0x005F8B7D) then free the sentinel node
// Leaf: ??1?$_List_base@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F900A, 23 bytes (`mov esi,ecx ; call clear ; if (sentinel) free(sentinel)`),
//   size-optimised TU (`pop ecx`).
// Identity (Codex name fix 2026-11-10): T = CWideString -- the landed CTCInventoryAbilities::ConstructScreen 0x005F519F destroys its
//   std::list<CWideString> titles local through this address; the member is STLport 4.6's ~_List_base (clear + deallocate;
//   ~list is empty and inline). Earlier landed as a placeholder (std::list_T_005f900a; the 165 byte-twins' propagated label
//   CFileInstaller::CActiveFile::OnReadFinished before that).
#pragma optimize("s", on)
class CWideString;
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
template std::_List_base<CWideString, std::allocator<CWideString> >::~_List_base();
