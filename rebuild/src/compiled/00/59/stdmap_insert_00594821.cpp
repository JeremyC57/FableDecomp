// std::map<long, NUISystem::CComponent*, std::less<long>, std::allocator<std::pair<long const, NUISystem::CComponent*> > >::insert(std::_Rb_tree_iterator<std::pair<long const, NUISystem::CComponent*> >, std::pair<long const, NUISystem::CComponent*> const&) -- STLport 4.6 insert (stlport/fable_map.h)
// Leaf: ?insert@?$map@JPAVCComponent@NUISystem@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJPAVCComponent@NUISystem@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJPAVCComponent@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJPAVCComponent@NUISystem@@@std@@@2@@2@U32@ABU?$pair@$$CBJPAVCComponent@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594821, oracle length 29 bytes.
// Identity: (1) the landed caller 0x005949C3 (std::map<long, NUISystem::CComponent *, std::less<long>, std::allocator<std::pair<long const, NUISystem::CComponent *>>>::operator[]) binds this map's insert (its iterator spelled without the traits argument: a binder spelling to fix, see NOTES) (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<JPAVCComponent@NUISystem@@> instantiation (_Tmap_traits@JPAVCComponent@NUISystem@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
typedef std::map<long, NUISystem::CComponent*> _Map_00594821;
template _Map_00594821::iterator _Map_00594821::insert(_Map_00594821::iterator, const _Map_00594821::value_type&);
