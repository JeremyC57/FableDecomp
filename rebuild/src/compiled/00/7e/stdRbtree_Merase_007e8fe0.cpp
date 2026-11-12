// std::_Rb_tree<CThing*, std::pair<CThing* const, float>, std::_Select1st<std::pair<CThing* const, float> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, float> > >::_M_erase(std::_Rb_tree_node<std::pair<CThing* const, float> >*) -- STLport 4.6 _M_erase (stlport/fable_map.h)
// Leaf: ?_M_erase@?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@M@std@@U?$_Select1st@U?$pair@QAVCThing@@M@std@@@3@U?$less@PAVCThing@@@3@V?$allocator@U?$pair@QAVCThing@@M@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@QAVCThing@@M@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8FE0, oracle length 51 bytes.
// Identity: (1) the respelled (proven) caller 0x007E99F0, this tree's insert_unique / erase, calls this address as this exact symbol (its STLport 4.6 out-of-line helper); (2) Ego_r.pdb / FableWin.pdb carry the same map instantiation (_Tmap_traits@PAVCThing@@MU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CThing;
template void std::_Rb_tree<CThing*, std::pair<CThing* const, float>, std::_Select1st<std::pair<CThing* const, float> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, float> > >::_M_erase(std::_Rb_tree_node<std::pair<CThing* const, float> >*);
