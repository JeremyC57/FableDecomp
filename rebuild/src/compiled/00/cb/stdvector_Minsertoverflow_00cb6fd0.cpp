// std::vector<std::pair<CCharString, CScriptInfo> >::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fillLength, bool atEnd) -- STLport 4.6 growth path of CScriptInfoManager's Scripts vector: allocate old + max(old, fillLength) elements, copy the head, construct the fill, copy the tail unless inserting at the end, then the out-of-line _M_clear and adopt the new block
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@VCCharString@@VCScriptInfo@@@std@@V?$allocator@U?$pair@VCCharString@@VCScriptInfo@@@std@@@2@@std@@IAEXPAU?$pair@VCCharString@@VCScriptInfo@@@2@ABU32@ABU__false_type@2@I_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB6FD0, thiscall `ret 14h`. Caller: vector::push_back 0x00CB7210 (landed, std spelling).
// Element: std::pair<CCharString, CScriptInfo> (24 bytes; Ego_r.pdb CScriptInfo 20 bytes: AllocFunc +0, MasterScriptDataAlloc +4,
//   Name +8, ID +0xC, MasterScript +0x10), the CVectorMap<CCharString, CScriptInfo> Scripts of CScriptInfoManager.
// Callees: malloc 0x00BFEA0E, the out-of-line __fastcall __uninitialized_copy 0x00CB60F0 (x2) and __uninitialized_fill_n 0x00CB6160
//   (FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N), CCharString copy ctor 0x0099EC30 (the fillLength == 1 construct), and the protected
//   _M_clear 0x00CB6E60 called out of line (FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL; the landed self row defines it).
// Body: the shared STLport 4.6 vector::_M_insert_overflow (stlport/fable_containers.h), explicitly instantiated, as the landed
//   0x00CBC130.
// Naming (NF180, 2026-11-20, q4077): was a row-local `ScriptVector` namespace model (its own CCharString / CScriptInfo / pair /
//   vector stand-ins, a public _M_clear and ScriptVector::uninitialized_copy / _fill_n helpers); now the shared headers, so the
//   _M_clear call binds the STLport spelling 0x00CB6E60 defines and the row's own leaf is the STLport instance spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CScriptInfo.h"
template void std::vector<std::pair<CCharString, CScriptInfo> >::_M_insert_overflow(std::pair<CCharString, CScriptInfo>*, const std::pair<CCharString, CScriptInfo>&, const std::__false_type&, unsigned int, bool);
