// std::map<CDateAndTime, CWideString>::insert(iterator position, const value_type& x) -- STLport 4.6 _map.h `return _M_t.insert_unique(position, x);` (the hinted insert out of line)
// Leaf: ?insert@?$map@VCDateAndTime@@VCWideString@@U?$less@VCDateAndTime@@@std@@V?$allocator@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCDateAndTime@@VCWideString@@@std@@@2@@2@U32@ABU?$pair@$$CBVCDateAndTime@@VCWideString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059483E, oracle length 29 bytes, thiscall, the iterator returned through the hidden pointer.
// Identity: (1) the landed caller 0x00594A08 (map::operator[]) calls this address as map<CDateAndTime, CWideString>::insert(iterator, const value_type&)
//   (rebuild/oracles/reloc_bindings.tsv; its spelling drops the allocator argument and the iterator's traits: a binder spelling to fix, see
//   NOTES); (2) Ego_r.pdb / FableWin.pdb carry map<CDateAndTime, CWideString> (_Tmap_traits@VCDateAndTime@@VCWideString@@U?$less@...).
// Model: the shared stlport/fable_map.h (STLport 4.6 map / _Rb_tree; FABLE_RETAIL_STLP_MAP); insert_unique(iterator, const V&) is declared only
//   (retail calls it out of line). Replaces the generated stand-in CForwarder_0059483e (was _unattributed::Forward_CForwarder_0059483e).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#include "engine/CDateAndTime.h"
#include "stlport/fable_map.h"
typedef std::map<CDateAndTime, CWideString> _Map_0059483e;
template _Map_0059483e::iterator _Map_0059483e::insert(_Map_0059483e::iterator, const _Map_0059483e::value_type&);
