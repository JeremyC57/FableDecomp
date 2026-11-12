// std::map<NUISystem::CComponent*, long>::insert(iterator position, const value_type& x) -- STLport 4.6 _map.h `return _M_t.insert_unique(position, x);` (the hinted insert out of line)
// Leaf: ?insert@?$map@PAVCComponent@NUISystem@@JU?$less@PAVCComponent@NUISystem@@@std@@V?$allocator@U?$pair@QAVCComponent@NUISystem@@J@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@J@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@J@std@@@2@@2@U32@ABU?$pair@QAVCComponent@NUISystem@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042CDC3, oracle length 29 bytes, thiscall, the iterator returned through the hidden pointer.
// Identity: (1) the landed caller 0x0042D1B7 (std::map<NUISystem::CComponent*,long,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent*_const,long>_>_>::operator[]) calls this address as map<NUISystem::CComponent*, long>::insert(iterator, const value_type&)
//   (rebuild/oracles/reloc_bindings.tsv; its spelling drops the allocator argument and the iterator's traits: a binder spelling to fix, see
//   NOTES); (2) Ego_r.pdb / FableWin.pdb carry map<NUISystem::CComponent*, long> (_Tmap_traits@PAVCComponent@NUISystem@@JU?$less@...).
// Model: the shared stlport/fable_map.h (STLport 4.6 map / _Rb_tree; FABLE_RETAIL_STLP_MAP); insert_unique(iterator, const V&) is declared only
//   (retail calls it out of line). Replaces the generated stand-in CForwarder_0042cdc3 (was _unattributed::Forward_CForwarder_0042cdc3).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"
typedef std::map<NUISystem::CComponent*, long> _Map_0042cdc3;
template _Map_0042cdc3::iterator _Map_0042cdc3::insert(_Map_0042cdc3::iterator, const _Map_0042cdc3::value_type&);
