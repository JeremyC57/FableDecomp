// std::_Rb_tree<long, std::pair<long const, CCountedPointer<CEngineSceneMap> >, std::_Select1st<std::pair<long const, CCountedPointer<CEngineSceneMap> > >, std::less<long>, std::allocator<std::pair<long const, CCountedPointer<CEngineSceneMap> > > >::insert_unique(std::pair<long const, CCountedPointer<CEngineSceneMap> > const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@U?$_Select1st@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B96FE0, oracle length 180 bytes.
// Identity: (1) the landed caller 0x00B975E0 (std::_Rb_tree::insert_unique) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<JV?$CCountedPointer@VCEngineSceneMap@@@@> instantiation (_Tmap_traits@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
class CEngineSceneMap;
template std::pair<std::_Rb_tree_iterator<std::pair<long const, CCountedPointer<CEngineSceneMap> >, std::_Nonconst_traits<std::pair<long const, CCountedPointer<CEngineSceneMap> > > >, bool> std::_Rb_tree<long, std::pair<long const, CCountedPointer<CEngineSceneMap> >, std::_Select1st<std::pair<long const, CCountedPointer<CEngineSceneMap> > >, std::less<long>, std::allocator<std::pair<long const, CCountedPointer<CEngineSceneMap> > > >::insert_unique(std::pair<long const, CCountedPointer<CEngineSceneMap> > const&);
