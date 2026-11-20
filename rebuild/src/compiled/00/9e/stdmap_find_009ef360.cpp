// std::map<CCharString, CCharString>::find(const CCharString& key) -- STLport 4.6 map lookup: iterator(_M_t._M_find(key)), the found node or end(), returned by value
// Leaf: ?find@?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V1@@std@@@2@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EF360, 21 bytes, thiscall, hidden iterator result ([esp+4]), key [esp+8], `ret 8`; the map
//   (whose _M_t is at +0) passes straight through to the tree's _M_find 0x009AB5D0 (out of line, node in eax).
// Identity: the landed RunCutsceneMacro 0x00CBFB7D, this row's only referencing row, binds this address as exactly this symbol
//   (reloc_bindings); the body is STLport 4.6 _map.h `iterator find(const key_type& __x) { return _M_t.find(__x); }` with the
//   tree's find inlined and its _M_find called -- the same shape as the landed set<CCharString>::find 0x004BAA30. The callee 0x009AB5D0 is the
//   landed _Rb_tree<CCharString, pair<const CCharString, CCharString>, ..>::_M_find<CCharString> (q4393's claim).
//   Descriptive-by-library (STLport instances are in neither PDB).
// Naming (NF180, 2026-11-20, q4393): was a stdcall stub labelled CRegion::SetMiniMapRegionExitTextOffsetX (a propagated label) over a
//   FableHelp_ stand-in for 0x009AB5D0; now the map member on the shared stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_MFIND_DECL: _M_find
//   called out of line), so the call binds the _M_find spelling the self row 0x009AB5D0 defines.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "fable_string.h"
#include "stlport/fable_map.h"
template std::map<CCharString, CCharString>::iterator std::map<CCharString, CCharString>::find(const CCharString&);
