// std::_Rb_tree<char, char, _Identity, less, allocator>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport 4.6 node insert (set<char>): create a node for v, link it left or right of y, fix root / leftmost / rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@DDU?$_Identity@D@std@@U?$less@D@2@V?$allocator@D@2@@std@@AAE?AU?$_Rb_tree_iterator@DU?$_Nonconst_traits@D@std@@@2@PAU_Rb_tree_node_base@2@0ABD0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513880 (`ret 14h`). Was the invented stand-in CMemoryAllocatorVariableSize::InsertNode*.
// Identity: the respelled caller 0x00514B50 (set<char>) (agentFAM12) calls this address as this private _M_insert (agentFAM12 CALLEE_FIXES.tsv;
//   the caller's types come from its landed binding + the Ego_r/FableWin name sets); the body is STLport 4.6 _tree.c _M_insert.
// Model: stlport/fable_map.h (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the STLport 4.6.2 body) + the explicit instantiation.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
template std::_Rb_tree<char, char, std::_Identity<char >, std::less<char >, std::allocator<char > >::iterator std::_Rb_tree<char, char, std::_Identity<char >, std::less<char >, std::allocator<char > >::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const char&, std::_Rb_tree_node_base*);
