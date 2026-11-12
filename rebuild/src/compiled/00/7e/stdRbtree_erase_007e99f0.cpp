// std::_Rb_tree<CThing*, std::pair<CThing* const, float>, std::_Select1st<std::pair<CThing* const, float> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, float> > >::erase(std::_Rb_tree_iterator<std::pair<CThing* const, float>, std::_Nonconst_traits<std::pair<CThing* const, float> > >, std::_Rb_tree_iterator<std::pair<CThing* const, float>, std::_Nonconst_traits<std::pair<CThing* const, float> > >) -- STLport 4.6 erase (stlport/fable_map.h)
// Leaf: ?erase@?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@M@std@@U?$_Select1st@U?$pair@QAVCThing@@M@std@@@3@U?$less@PAVCThing@@@3@V?$allocator@U?$pair@QAVCThing@@M@std@@@3@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@QAVCThing@@M@std@@U?$_Nonconst_traits@U?$pair@QAVCThing@@M@std@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E99F0, oracle length 189 bytes.
// Identity: (1) the landed caller 0x007E9D80 (std::_Rb_tree<CThing *, std::pair<CThing *const, float>, std::_Select1st<std::pair<CThing *const, float>>, std::less<CThing *>, std::allocator<std::pair<CThing *const, float>>>::erase) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<PAVCThing@@M> instantiation (_Tmap_traits@PAVCThing@@MU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "stlport/fable_map.h"
class CThing;
template void std::_Rb_tree<CThing*, std::pair<CThing* const, float>, std::_Select1st<std::pair<CThing* const, float> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, float> > >::erase(std::_Rb_tree_iterator<std::pair<CThing* const, float>, std::_Nonconst_traits<std::pair<CThing* const, float> > >, std::_Rb_tree_iterator<std::pair<CThing* const, float>, std::_Nonconst_traits<std::pair<CThing* const, float> > >);
