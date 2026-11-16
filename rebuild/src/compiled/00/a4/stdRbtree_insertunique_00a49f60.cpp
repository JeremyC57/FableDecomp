// std::_Rb_tree<unsigned long, pair<const unsigned long, CCharString>, ..>::insert_unique(iterator position, const value_type& v) -- STLport hinted unique insert of the lip-sync phoneme-symbol map
// Flags: none
// Leaf: ?insert_unique@?$_Rb_tree@KU?$pair@$$CBKVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBKVCCharString@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCCharString@@@std@@@2@@2@U32@ABU?$pair@$$CBKVCCharString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A49F60 (the out-of-line COMDAT map<unsigned long, CCharString>::operator[] 0x00A4A670
//   calls), `ret 8` with the hidden iterator result.
// Name (round 4, NF40): Codex pass 22 (landed_evidence/00a495f0 NAME_FIXES.tsv: whole native body and every named relocation
//   exact) names it the STLport 4.6 _tree.c hinted insert_unique of _Rb_tree<unsigned long, pair<const unsigned long, CCharString>,
//   _Select1st<..>, less<unsigned long>, allocator<..> >. Was a local HeroMorphTree:: model (catalog BST_InsertEntry_EHeroMorphType,
//   propagated) binding stand-in iterator / _M_insert spellings; now the explicit instantiation of stlport/fable_map.h's hinted
//   body (FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY, the 0x00A495F0 TU's opt-ins). It binds insert_unique(const V&) 0x00632C90,
//   _M_insert 0x00632D30 and the iterator operator++ / operator-- COMDATs 0x00A48580 / 0x00A48750 by their STLport names.
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
template std::_Rb_tree_iterator<std::pair<const unsigned long, CCharString>, std::_Nonconst_traits<std::pair<const unsigned long, CCharString> > >
std::_Rb_tree<unsigned long, std::pair<const unsigned long, CCharString>, std::_Select1st<std::pair<const unsigned long, CCharString> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CCharString> > >::insert_unique(iterator, const std::pair<const unsigned long, CCharString>&);
