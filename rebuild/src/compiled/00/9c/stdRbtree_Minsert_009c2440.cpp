// std::_Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, _Identity, less, allocator>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport 4.6 node insert (set<ESurfaceMultisampleType>): create a node for v, link it left or right of y, fix root / leftmost / rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4ESurfaceMultisampleType@@W41@U?$_Identity@W4ESurfaceMultisampleType@@@std@@U?$less@W4ESurfaceMultisampleType@@@3@V?$allocator@W4ESurfaceMultisampleType@@@3@@std@@AAE?AU?$_Rb_tree_iterator@W4ESurfaceMultisampleType@@U?$_Nonconst_traits@W4ESurfaceMultisampleType@@@std@@@2@PAU_Rb_tree_node_base@2@0ABW4ESurfaceMultisampleType@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C2440 (`ret 14h`). Was the invented stand-in CMemoryAllocatorVariableSize::InsertNode*.
// Identity: the respelled caller 0x009C29B0 (set<ESurfaceMultisampleType>) (agentFAM12) calls this address as this private _M_insert (agentFAM12 CALLEE_FIXES.tsv;
//   the caller's types come from its landed binding + the Ego_r/FableWin name sets); the body is STLport 4.6 _tree.c _M_insert.
// Model: stlport/fable_map.h (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the STLport 4.6.2 body) + the explicit instantiation.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/ESurfaceMultisampleType.h"
template std::_Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, std::_Identity<ESurfaceMultisampleType >, std::less<ESurfaceMultisampleType >, std::allocator<ESurfaceMultisampleType > >::iterator std::_Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, std::_Identity<ESurfaceMultisampleType >, std::less<ESurfaceMultisampleType >, std::allocator<ESurfaceMultisampleType > >::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const ESurfaceMultisampleType&, std::_Rb_tree_node_base*);
