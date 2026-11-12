// std::_Rb_tree<char, char, std::_Identity<char>, std::less<char>, std::allocator<char> >::insert_unique(char const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@DDU?$_Identity@D@std@@U?$less@D@2@V?$allocator@D@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@DU?$_Nonconst_traits@D@std@@@std@@_N@2@ABD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00514B50, oracle length 180 bytes.
// Identity: (1) the landed caller 0x004FE7F0 (CWorldMap::GetSoundThemesUsedInCurrentRegion) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the set<D> instantiation (_Tset_traits@DU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

template std::pair<std::_Rb_tree_iterator<char, std::_Nonconst_traits<char> >, bool> std::_Rb_tree<char, char, std::_Identity<char>, std::less<char>, std::allocator<char> >::insert_unique(char const&);
