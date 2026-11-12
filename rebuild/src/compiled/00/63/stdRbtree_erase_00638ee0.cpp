// std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long> >::erase(std::_Rb_tree_iterator<long, std::_Nonconst_traits<long> >, std::_Rb_tree_iterator<long, std::_Nonconst_traits<long> >) -- STLport 4.6 erase (stlport/fable_map.h)
// Leaf: ?erase@?$_Rb_tree@JJU?$_Identity@J@std@@U?$less@J@2@V?$allocator@J@2@@std@@QAEXU?$_Rb_tree_iterator@JU?$_Nonconst_traits@J@std@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00638EE0, oracle length 189 bytes.
// Identity: (1) the landed caller 0x00639590 (std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long>>::erase) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the set<J> instantiation (_Tset_traits@JU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "stlport/fable_map.h"

template void std::_Rb_tree<long, long, std::_Identity<long>, std::less<long>, std::allocator<long> >::erase(std::_Rb_tree_iterator<long, std::_Nonconst_traits<long> >, std::_Rb_tree_iterator<long, std::_Nonconst_traits<long> >);
