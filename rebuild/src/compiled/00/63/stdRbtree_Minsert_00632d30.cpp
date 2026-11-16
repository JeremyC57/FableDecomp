// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- (map<unsigned long, CCharString>) STLport 4.6 node insert: create a node for v, link it left or right of y, fix root / leftmost / rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@KU?$pair@$$CBKVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBKVCCharString@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCCharString@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBKVCCharString@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632D30, 202 bytes (+ int3 padding to 208), thiscall `ret 14h` (the hidden
//   iterator result pointer first, then x, y, &v, w).
// Identity (proven): map<unsigned long, CCharString>; evidence: 0x18-byte nodes: a 4-byte key compared unsigned and a CCharString value copied by 0x0099EC30; the unsigned 4-byte-key CCharString maps of both PDB name sets are all map<unsigned long, CCharString> (_Tmap_traits@KVCCharString; e.g. Ego_r.pdb CUIDef::ActionMap), whose one COMDAT this is -- its operator[] 0x00632C90 serves the CUIDef code and CLipSyncSampleData's ReadFromFile / WriteToFile maps (0x00A49F60 / 0x00A4A670)
// Label: the manifest's "List_InsertNode" is descriptive (family J098: five rows that differ only in the key compare width,
//   the node size and the value copy call; retail has no identical-code folding, so each address is one instantiation);
//   this is STLport 4.6 `_Rb_tree::_M_insert` (`_tree.c`).
// Callees (this instantiation): malloc 0x00BFEA0E (inline allocate, 0x18-byte node), the value copy CCharString::CCharString(const CCharString&)
//   @ 0x0099EC30 (thiscall on node+0x14, &v.second), _Rb_global<bool>::_Rebalance 0x0042971D (__cdecl).
// Model: the landed _M_insert 0x006BD140 (STLport 4.6 verbatim) over the five-parameter _Rb_tree; the pair copy
//   constructor inlines into _M_create_node as the null-checked placement copy (key word, then the out-of-line value copy).
// Port (round 4, NF40): the local std::_Rb_tree / pair / iterator / CCharString models are replaced by the explicit
//   instantiation of stlport/fable_map.h's the private _M_insert (STLport 4.6.2 _tree.c, FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY): AAE (private), which the landed row spelled public (QAE) over fable_string.h, with the opt-ins of the landed
//   0x00A495F0 TU (Codex pass 22 typed chain: map<unsigned long, CCharString> of the lip-sync sample data). The iterator is
//   _Rb_tree_iterator<V, _Nonconst_traits<V> >, as the queue claim and the PDB-shaped callers spell it.
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"

typedef std::_Rb_tree<unsigned long, std::pair<const unsigned long, CCharString>, std::_Select1st<std::pair<const unsigned long, CCharString> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CCharString> > > TTree;

template TTree::iterator TTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const std::pair<const unsigned long, CCharString>&, std::_Rb_tree_node_base*);
