// std::vector<std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> >::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fillLength, bool atEnd) -- STLport 4.6 growth path of the runtime scripted-entity persist map's vector: allocate old + max(old, fillLength) elements, copy the head, construct the fill, copy the tail unless inserting at the end, destroy + free the old block and adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBC130, 284 bytes, `ret 14h`. Callers: CRuntimeScriptedEntityPersist::LoadFromGameStateScript
//   0x00CB8440 (0x00CB85CC) and vector::push_back 0x00CBC7B0 (0x00CBC7FE).
// Element: Ego_r.pdb std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (24 bytes, 8-aligned): first +0,
//   CPersistInfo second +8 (UID unsigned __int64 +0, PersistScript CCharString +8, 16 bytes); the CVectorMap<unsigned __int64,
//   CPersistInfo> PersistScripts of CRuntimeScriptedEntityPersist. The implicit pair / CPersistInfo copy gives retail's four dword
//   moves and the CCharString copy ctor at +0x10.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, the out-of-line __fastcall __uninitialized_copy 0x00CB9B00 and
//   __uninitialized_fill_n 0x00CB9B50 (FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N), CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0.
// Body: the shared STLport 4.6 vector::_M_insert_overflow (stlport/fable_containers.h), explicitly instantiated.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CRuntimeScriptedEntityPersist.h"
template void std::vector<std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> >::_M_insert_overflow(std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*, const std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>&, const std::__false_type&, unsigned int, bool);