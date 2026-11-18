// std::map<unsigned long, NUISystem::CUIState>::map(const map& x) -- STLport 4.6 map copy constructor with the tree copy constructor
// expanded: allocate the header node, then either mark the tree empty or clone the other tree's nodes and set leftmost / rightmost,
// finally take the node count
// Leaf: ??0?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CB9B, length 116 bytes, `ret 4`; size TU.
// Callees: malloc @ 0x00BFEA0E (the 0x38-byte header node), _Rb_tree::_M_copy @ 0x0042CB90 (landed), _Rb_tree_node_base::_S_minimum /
//   _S_maximum @ 0x0041C7E0 / 0x0041C800 (landed; out of line here, __cdecl).
// Identity (NF84, round 7, q10659 self, NF64-blocked): the landed callers bind this address as the map copy constructor
//   ??0?$map@KVCUIState@NUISystem@@...@QAE@ABV01@@Z (reloc_bindings: 0x0053CEF6; NF64 probed 0x0053CCA5 the same); STLport 4.6's map
//   copy constructor `map(const _Self& __x) : _M_t(__x._M_t) {}` is this body once the tree copy constructor is expanded. Same tree as
//   the landed _M_copy 0x0042CB90 / operator= 0x0042CE7A. Descriptive by library (STLport; PDBs are Dinkumware). Was the stand-in
//   std::_Rb_tree::_Rb_tree (a local STLport model, leaf ??0?$_Rb_tree@...).
// Port (NF84): stlport/fable_map.h, NF78's TREE_COPY_CTOR_BODY + MAP_COPY_CTOR_BODY (the 0x004F97B3 recipe) with the NF84 overlay opt-in
//   FABLE_RETAIL_STLP_TREE_COPY_CTOR_SMINMAX (the leftmost / rightmost walks as the out-of-line node-base calls), engine/NUISystem_CUIState.h.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY
#define FABLE_RETAIL_STLP_TREE_COPY_CTOR_SMINMAX
#define FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY
#include "fable_compiler.h"
#include "engine/NUISystem_CUIState.h"
#include "stlport/fable_map.h"

template std::map<unsigned long, NUISystem::CUIState>::map(const std::map<unsigned long, NUISystem::CUIState>&);
