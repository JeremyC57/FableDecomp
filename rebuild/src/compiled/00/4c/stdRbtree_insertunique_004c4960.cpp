// std::_Rb_tree<void*, std::pair<void* const, CRGBColour>, std::_Select1st<std::pair<void* const, CRGBColour> >, std::less<void*>, std::allocator<std::pair<void* const, CRGBColour> > >::insert_unique(std::pair<void* const, CRGBColour> const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@PAXU?$pair@QAXVCRGBColour@@@std@@U?$_Select1st@U?$pair@QAXVCRGBColour@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCRGBColour@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@QAXVCRGBColour@@@std@@U?$_Nonconst_traits@U?$pair@QAXVCRGBColour@@@std@@@2@@std@@_N@2@ABU?$pair@QAXVCRGBColour@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C4960, oracle length 180 bytes.
// Identity: (1) the landed caller 0x004C4D50 (std::_Rb_tree::insert_unique) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<PAXVCRGBColour@@> instantiation (_Tmap_traits@PAXVCRGBColour@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CRGBColour.h"
template std::pair<std::_Rb_tree_iterator<std::pair<void* const, CRGBColour>, std::_Nonconst_traits<std::pair<void* const, CRGBColour> > >, bool> std::_Rb_tree<void*, std::pair<void* const, CRGBColour>, std::_Select1st<std::pair<void* const, CRGBColour> >, std::less<void*>, std::allocator<std::pair<void* const, CRGBColour> > >::insert_unique(std::pair<void* const, CRGBColour> const&);
