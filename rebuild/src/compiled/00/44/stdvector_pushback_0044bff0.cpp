// std::vector<CCharString>::push_back(const CCharString& x) -- STLport 4.6: copy-construct x at the finish when there is room,
//   else grow through _M_insert_overflow (one copy, at the end)
// Leaf: ?push_back@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044BFF0, thiscall `ret 4`: CCharString(const CCharString&) 0x0099EC30 under the placement-new
//   null test, `add [esi+4],4`; else _M_insert_overflow 0x00433530 (finish, x, the __false_type temporary homed in x's argument slot,
//   1, true).
// Name fix (NF208, 2026-11-20, FP-VECTORINSERT q6332 / q6341): was a Leaf-less free __fastcall `push_back(VectorPushBackOverlay*, void*,
//   CWideString*)` over a header-less overlay whose "CWideString" copy is CCharString's copy constructor 0x0099EC30 and whose
//   InsertRange_00433530 is vector<CCharString>::_M_insert_overflow. Now the STLport member from fable_string.h +
//   stlport/fable_containers.h, emitted by an explicit instantiation (the 0x004128A0 vector<CWideString> precedent), with the overflow
//   declaration-only (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY): the spelling reloc_bindings binds at 0x0044BFF0 (0x00CBFACA) and
//   the one every includer of the shared vector<CCharString> emits pick-any. Descriptive-by-library (STL instance).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"
template void std::vector<CCharString>::push_back(const CCharString&);
