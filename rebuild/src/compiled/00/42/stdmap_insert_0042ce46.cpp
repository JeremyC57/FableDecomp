// std::map<long, EEngine2DLayer>::insert(iterator position, const value_type& x) -- STLport 4.6 _map.h `return _M_t.insert_unique(position, x);` (the hinted insert out of line)
// Leaf: ?insert@?$map@JW4EEngine2DLayer@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@2@U32@ABU?$pair@$$CBJW4EEngine2DLayer@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042CE46, oracle length 29 bytes, thiscall, the iterator returned through the hidden pointer.
// Identity: (1) the landed caller 0x0042D246 (std::map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>::operator[]) calls this address as map<long, EEngine2DLayer>::insert(iterator, const value_type&)
//   (rebuild/oracles/reloc_bindings.tsv; its spelling drops the allocator argument and the iterator's traits: a binder spelling to fix, see
//   NOTES); (2) Ego_r.pdb / FableWin.pdb carry map<long, EEngine2DLayer> (_Tmap_traits@JW4EEngine2DLayer@@U?$less@...).
// Model: the shared stlport/fable_map.h (STLport 4.6 map / _Rb_tree; FABLE_RETAIL_STLP_MAP); insert_unique(iterator, const V&) is declared only
//   (retail calls it out of line). Replaces the generated stand-in CForwarder_0042ce46 (was _unattributed::Forward_CForwarder_0042ce46).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
enum EEngine2DLayer { };
#include "stlport/fable_map.h"
typedef std::map<long, EEngine2DLayer> _Map_0042ce46;
template _Map_0042ce46::iterator _Map_0042ce46::insert(_Map_0042ce46::iterator, const _Map_0042ce46::value_type&);
