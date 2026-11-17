// std::__uninitialized_fill_n(map<unsigned long, NUISystem::CUIState>* first, unsigned int n, const map& x, const __false_type&) -- STLport 4.6
//   _uninitialized.h: copy-construct n copies of x in place (the map copy constructor 0x0053CB9B, placement new), return the advanced pointer
// Leaf: ??$__uninitialized_fill_n@PAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@IV12@@std@@YIPAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@0@PAV10@IABV10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CCD0, 38 bytes, __fastcall (first ecx, n edx, the value and tag references on the stack), `ret 8`.
// Identity (NF86, round 7, q11118 binder): its only caller, vector<map<unsigned long, CUIState> >::_M_insert_overflow 0x0053CEF6 (landed),
//   binds exactly this STLport spelling here, and the body is that loop over the 12-byte map element calling the map copy constructor
//   0x0053CB9B (NF84's self row ??0?$map@KVCUIState@NUISystem@@..@QAE@ABV01@@Z). Descriptive by library (STLport 4.6; both PDBs are Dinkumware).
//   Was the generated stand-in _global::UninitFillN_CElement12_0053cb9b_0053ccd0 (a local 12-byte element; the copy ctor bound as
//   ??0CElement12_0053cb9b@@QAE@ABU0@@Z).
// Port (NF86): one explicit instantiation on stlport/fable_containers.h (FABLE_STLP_OUTOFLINE_UNINIT_FILL_N: the __fastcall out-of-line COMDAT)
//   + stlport/fable_map.h (the out-of-line map copy constructor, as the TU-mate 0x0053CCA5 spells it) + engine/NUISystem_CUIState.h.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CUIState.h"

typedef std::map<unsigned long, NUISystem::CUIState> CUIStateMap;
template CUIStateMap* __fastcall std::__uninitialized_fill_n(CUIStateMap* first, unsigned int n, const CUIStateMap& x, const std::__false_type&);
