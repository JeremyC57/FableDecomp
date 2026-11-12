// std::_Rb_tree<long, std::pair<long const, unsigned long>, std::_Select1st<std::pair<long const, unsigned long> >, std::less<long>, std::allocator<std::pair<long const, unsigned long> > >::insert_unique(std::pair<long const, unsigned long> const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJK@std@@U?$_Select1st@U?$pair@$$CBJK@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJK@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJK@std@@U?$_Nonconst_traits@U?$pair@$$CBJK@std@@@2@@std@@_N@2@ABU?$pair@$$CBJK@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E91C0, oracle length 180 bytes.
// Identity: (1) the landed caller 0x005E93E0 (std::_Rb_tree::insert_unique) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<JK> instantiation (_Tmap_traits@JKU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

template std::pair<std::_Rb_tree_iterator<std::pair<long const, unsigned long>, std::_Nonconst_traits<std::pair<long const, unsigned long> > >, bool> std::_Rb_tree<long, std::pair<long const, unsigned long>, std::_Select1st<std::pair<long const, unsigned long> >, std::less<long>, std::allocator<std::pair<long const, unsigned long> > >::insert_unique(std::pair<long const, unsigned long> const&);
