// std::map<long, long, std::less<long>, std::allocator<std::pair<long const, long> > >::insert(std::_Rb_tree_iterator<std::pair<long const, long> >, std::pair<long const, long> const&) -- STLport 4.6 insert (stlport/fable_map.h)
// Leaf: ?insert@?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJJ@std@@U?$_Nonconst_traits@U?$pair@$$CBJJ@std@@@2@@2@U32@ABU?$pair@$$CBJJ@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B2EA7, oracle length 29 bytes.
// Identity: (1) the landed caller 0x005B30BB (std::map<long, long, std::less<long>, std::allocator<std::pair<long const, long>>>::operator[]) binds this map's insert (its iterator spelled without the traits argument: a binder spelling to fix, see NOTES) (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<JJ> instantiation (_Tmap_traits@JJU?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

typedef std::map<long, long> _Map_005b2ea7;
template _Map_005b2ea7::iterator _Map_005b2ea7::insert(_Map_005b2ea7::iterator, const _Map_005b2ea7::value_type&);
