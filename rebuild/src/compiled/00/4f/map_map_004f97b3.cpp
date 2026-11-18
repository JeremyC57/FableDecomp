// std::map<EDamageAttribute, float>::map(const map& x) -- STLport map copy constructor (the tree copy constructor expanded): allocate the header node, then either mark the tree empty or clone the other tree's nodes (_M_copy) and set leftmost / rightmost, finally take the node count
// Leaf: ??0?$map@W4EDamageAttribute@@MU?$less@W4EDamageAttribute@@@std@@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F97B3, length 126 bytes (ret 4), a COMDAT in a size-optimised TU.
// Identity (NF78, q10948): the map<EDamageAttribute,float> (Ego_r.pdb CComboMultiplierData::ComboMultiplier / CHitParameters::DamageMap)
//   copy constructor: its _M_copy callee 0x00461533 is that tree's _M_copy (catalog), the landed 0x006DCB20 (_M_insert of the
//   map<long, CComboMultiplierData> tree) and NF75's 0x004F9784 (pair<long const, CComboMultiplierData> copy) bind this spelling.
//   Descriptive-by-library: STLport instance, no PDB holds it (both PDBs are Dinkumware builds). Was the namespaced stand-in
//   NPairLongMapTree::_Rb_tree::_Rb_tree.
// Callees: malloc @ 0x00BFEA0E (the 0x18-byte header node), _Rb_tree::_M_copy(x, p) @ 0x00461533.
// Port (NF78): stlport/fable_map.h with the NF78 overlay opt-ins FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY (STLport _tree.h copy
//   constructor) and FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY (STLport _map.h copy constructor); one explicit instantiation.
// Inline-budget: FABLE_FORCEINLINE on the tree copy constructor (overlay opt-in FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY, EXTRA_RULES
//   2026-11-08 preference 3) -- retail expands _Rb_tree(const _Rb_tree&) inside this map copy constructor; VC7.1 /Os keeps it out of
//   line at the default budget, /d2inlS22 and /d2inlS60 (18 v 126 bytes each, grades 1-3); forced: RM (grade 4).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY
#define FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"

template std::map<EDamageAttribute, float>::map(const std::map<EDamageAttribute, float>&);
