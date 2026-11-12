// std::_Rb_tree<void*, std::pair<void* const, CRGBColour>, std::_Select1st<std::pair<void* const, CRGBColour> >, std::less<void*>, std::allocator<std::pair<void* const, CRGBColour> > >::erase(std::_Rb_tree_iterator<std::pair<void* const, CRGBColour>, std::_Nonconst_traits<std::pair<void* const, CRGBColour> > >, std::_Rb_tree_iterator<std::pair<void* const, CRGBColour>, std::_Nonconst_traits<std::pair<void* const, CRGBColour> > >) -- STLport 4.6 erase (stlport/fable_map.h)
// Leaf: ?erase@?$_Rb_tree@PAXU?$pair@QAXVCRGBColour@@@std@@U?$_Select1st@U?$pair@QAXVCRGBColour@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCRGBColour@@@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@QAXVCRGBColour@@@std@@U?$_Nonconst_traits@U?$pair@QAXVCRGBColour@@@std@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3F90, oracle length 189 bytes.
// Identity: (1) the landed caller 0x004C4430 (std::_Rb_tree<void *, std::pair<void *const, CRGBColour>, std::_Select1st<std::pair<void *const, CRGBColour>>, std::less<void *>, std::allocator<std::pair<void *const, CRGBColour>>>::erase) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<PAXVCRGBColour@@> instantiation (_Tmap_traits@PAXVCRGBColour@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "stlport/fable_map.h"
#include "engine/CRGBColour.h"
template void std::_Rb_tree<void*, std::pair<void* const, CRGBColour>, std::_Select1st<std::pair<void* const, CRGBColour> >, std::less<void*>, std::allocator<std::pair<void* const, CRGBColour> > >::erase(std::_Rb_tree_iterator<std::pair<void* const, CRGBColour>, std::_Nonconst_traits<std::pair<void* const, CRGBColour> > >, std::_Rb_tree_iterator<std::pair<void* const, CRGBColour>, std::_Nonconst_traits<std::pair<void* const, CRGBColour> > >);
