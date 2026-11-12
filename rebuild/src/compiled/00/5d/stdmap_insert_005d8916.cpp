// std::map<unsigned long, long, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, long> > >::insert(std::_Rb_tree_iterator<std::pair<unsigned long const, long>, std::_Nonconst_traits<std::pair<unsigned long const, long> > >, std::pair<unsigned long const, long> const&) -- STLport 4.6 insert (stlport/fable_map.h)
// Leaf: ?insert@?$map@KJU?$less@K@std@@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKJ@std@@U?$_Nonconst_traits@U?$pair@$$CBKJ@std@@@2@@2@U32@ABU?$pair@$$CBKJ@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8916, oracle length 29 bytes.
// Identity: (1) the landed caller 0x005D8933 (std::map<unsigned long, long, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, long>>>::operator[]) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<KJ> instantiation (_Tmap_traits@KJU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

template std::_Rb_tree_iterator<std::pair<unsigned long const, long>, std::_Nonconst_traits<std::pair<unsigned long const, long> > > std::map<unsigned long, long, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, long> > >::insert(std::_Rb_tree_iterator<std::pair<unsigned long const, long>, std::_Nonconst_traits<std::pair<unsigned long const, long> > >, std::pair<unsigned long const, long> const&);
