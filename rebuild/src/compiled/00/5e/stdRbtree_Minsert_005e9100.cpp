// std::_Rb_tree<long, std::pair<long const, unsigned long>, std::_Select1st<std::pair<long const, unsigned long> >, std::less<long>, std::allocator<std::pair<long const, unsigned long> > >::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, std::pair<long const, unsigned long> const&, std::_Rb_tree_node_base*) -- STLport 4.6 _M_insert (stlport/fable_map.h)
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJK@std@@U?$_Select1st@U?$pair@$$CBJK@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJK@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJK@std@@U?$_Nonconst_traits@U?$pair@$$CBJK@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJK@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E9100, oracle length 191 bytes.
// Identity: (1) the respelled (proven) caller 0x005E91C0, map<long, unsigned long> insert_unique, calls this address as this exact symbol (its STLport 4.6 out-of-line _M_insert); (2) Ego_r.pdb / FableWin.pdb carry map<long, unsigned long> (_Tmap_traits@JKU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

template std::_Rb_tree_iterator<std::pair<long const, unsigned long>, std::_Nonconst_traits<std::pair<long const, unsigned long> > > std::_Rb_tree<long, std::pair<long const, unsigned long>, std::_Select1st<std::pair<long const, unsigned long> >, std::less<long>, std::allocator<std::pair<long const, unsigned long> > >::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, std::pair<long const, unsigned long> const&, std::_Rb_tree_node_base*);
