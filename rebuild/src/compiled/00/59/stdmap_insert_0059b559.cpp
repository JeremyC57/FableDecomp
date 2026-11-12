// std::map<FRONTEND_SCREEN, NUISystem::CComponent*, std::less<FRONTEND_SCREEN>, std::allocator<std::pair<FRONTEND_SCREEN const, NUISystem::CComponent*> > >::insert(std::_Rb_tree_iterator<std::pair<FRONTEND_SCREEN const, NUISystem::CComponent*> >, std::pair<FRONTEND_SCREEN const, NUISystem::CComponent*> const&) -- STLport 4.6 insert (stlport/fable_map.h)
// Leaf: ?insert@?$map@W4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@U?$less@W4FRONTEND_SCREEN@@@std@@V?$allocator@U?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@std@@@5@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@std@@@2@@2@U32@ABU?$pair@$$CBW4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B559, oracle length 29 bytes.
// Identity: (1) the landed caller 0x0059B5D7 (std::map<FRONTEND_SCREEN, NUISystem::CComponent *, std::less<FRONTEND_SCREEN>, std::allocator<std::pair<FRONTEND_SCREEN const, NUISystem::CComponent *>>>::operator[]) binds this map's insert (its iterator spelled without the traits argument: a binder spelling to fix, see NOTES) (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<W4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@> instantiation (_Tmap_traits@W4FRONTEND_SCREEN@@PAVCComponent@NUISystem@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
enum FRONTEND_SCREEN { };
namespace NUISystem { class CComponent; }
typedef std::map<FRONTEND_SCREEN, NUISystem::CComponent*> _Map_0059b559;
template _Map_0059b559::iterator _Map_0059b559::insert(_Map_0059b559::iterator, const _Map_0059b559::value_type&);
