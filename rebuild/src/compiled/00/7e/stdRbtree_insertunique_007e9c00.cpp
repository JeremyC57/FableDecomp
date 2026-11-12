// std::_Rb_tree<CThing*, std::pair<CThing* const, float>, std::_Select1st<std::pair<CThing* const, float> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, float> > >::insert_unique(std::pair<CThing* const, float> const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@M@std@@U?$_Select1st@U?$pair@QAVCThing@@M@std@@@3@U?$less@PAVCThing@@@3@V?$allocator@U?$pair@QAVCThing@@M@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@QAVCThing@@M@std@@U?$_Nonconst_traits@U?$pair@QAVCThing@@M@std@@@2@@std@@_N@2@ABU?$pair@QAVCThing@@M@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E9C00, oracle length 180 bytes.
// Identity: (1) the landed caller 0x007E9DE0 (std::_Rb_tree::insert_unique) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<PAVCThing@@M> instantiation (_Tmap_traits@PAVCThing@@MU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CThing;
template std::pair<std::_Rb_tree_iterator<std::pair<CThing* const, float>, std::_Nonconst_traits<std::pair<CThing* const, float> > >, bool> std::_Rb_tree<CThing*, std::pair<CThing* const, float>, std::_Select1st<std::pair<CThing* const, float> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, float> > >::insert_unique(std::pair<CThing* const, float> const&);
