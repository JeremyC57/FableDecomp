// std::map<NUISystem::CComponent*, std::pair<CWideString, long>>::insert(iterator position, const value_type& x) -- STLport 4.6 _map.h `return _M_t.insert_unique(position, x);` (the hinted insert out of line)
// Leaf: ?insert@?$map@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@2@@2@U32@ABU?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B2EC4, oracle length 29 bytes, thiscall, the iterator returned through the hidden pointer.
// Identity: (1) the landed caller 0x005B3100 (std::map<CComponent*, pair<CWideString, long> >::operator[]) calls this address as map<NUISystem::CComponent*, std::pair<CWideString, long>>::insert(iterator, const value_type&)
//   (rebuild/oracles/reloc_bindings.tsv; its spelling drops the allocator argument and the iterator's traits: a binder spelling to fix, see
//   NOTES); (2) Ego_r.pdb / FableWin.pdb carry map<NUISystem::CComponent*, std::pair<CWideString, long>> (_Tmap_traits@PAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@U?$less@...).
// Model: the shared stlport/fable_map.h (STLport 4.6 map / _Rb_tree; FABLE_RETAIL_STLP_MAP); insert_unique(iterator, const V&) is declared only
//   (retail calls it out of line). Replaces the generated stand-in CForwarder_005b2ec4 (was _unattributed::Forward_CForwarder_005b2ec4).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"
typedef std::map<NUISystem::CComponent*, std::pair<CWideString, long> > _Map_005b2ec4;
template _Map_005b2ec4::iterator _Map_005b2ec4::insert(_Map_005b2ec4::iterator, const _Map_005b2ec4::value_type&);
