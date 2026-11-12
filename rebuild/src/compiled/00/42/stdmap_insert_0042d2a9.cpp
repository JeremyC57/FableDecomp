// std::map<CCharString, CCountedPointer<NUISystem::CComponent>>::insert(iterator position, const value_type& x) -- STLport 4.6 _map.h `return _M_t.insert_unique(position, x);` (the hinted insert out of line)
// Leaf: ?insert@?$map@VCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D2A9, oracle length 29 bytes, thiscall, the iterator returned through the hidden pointer.
// Identity: (1) the landed caller 0x0042D45D (std::map<CCharString, CCountedPointer<NUISystem::CComponent> >::operator[]) calls this address as map<CCharString, CCountedPointer<NUISystem::CComponent>>::insert(iterator, const value_type&)
//   (rebuild/oracles/reloc_bindings.tsv; its spelling drops the allocator argument and the iterator's traits: a binder spelling to fix, see
//   NOTES); (2) Ego_r.pdb / FableWin.pdb carry map<CCharString, CCountedPointer<NUISystem::CComponent>> (_Tmap_traits@VCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@U?$less@...).
// Model: the shared stlport/fable_map.h (STLport 4.6 map / _Rb_tree; FABLE_RETAIL_STLP_MAP); insert_unique(iterator, const V&) is declared only
//   (retail calls it out of line). Replaces the generated stand-in CForwarder_0042d2a9 (was _unattributed::Forward_CForwarder_0042d2a9).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"
typedef std::map<CCharString, CCountedPointer<NUISystem::CComponent> > _Map_0042d2a9;
template _Map_0042d2a9::iterator _Map_0042d2a9::insert(_Map_0042d2a9::iterator, const _Map_0042d2a9::value_type&);
