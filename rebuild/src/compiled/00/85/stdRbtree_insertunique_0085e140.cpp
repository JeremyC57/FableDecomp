// std::_Rb_tree<CTCMapwho*, CTCMapwho*, _Identity<CTCMapwho* >, less<CTCMapwho* >, allocator<CTCMapwho* > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with the instance's iterator operator-- (0x0085DAA0, out of line) unless at begin(), then _M_insert (0x0085DED0) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@PAVCTCMapwho@@PAV1@U?$_Identity@PAVCTCMapwho@@@std@@U?$less@PAVCTCMapwho@@@3@V?$allocator@PAVCTCMapwho@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCTCMapwho@@U?$_Nonconst_traits@PAVCTCMapwho@@@std@@@std@@_N@2@ABQAVCTCMapwho@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085E140, 180 bytes, thiscall `ret 8`; /O2 TU.
// Identity (NF58, round 7): the same tree as its callee _M_insert 0x0085DED0 (see that row); its landed caller is NThingMap::CThingMap::GetAllThings 0x0085D010 (FableWin.pdb ?GetAllThings@CThingMap@NThingMap@@QAEXAAV?$set@PAVCTCMapwho@@...@Z).
// Was the stand-in `_unattributed::InsertUnique_CRetailTree_0085e140` (local model); now the explicit instantiation of stlport/fable_map.h's insert_unique (FABLE_RETAIL_STLP_MAP
//   body) with the iterator's operator-- kept out of line (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE, as retail calls the per-instance
//   COMDAT 0x0085DAA0). STLport 4.6.2 source, descriptive-by-library.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
class CTCMapwho;
namespace std {
template pair<_Rb_tree<CTCMapwho*, CTCMapwho*, _Identity<CTCMapwho* >, less<CTCMapwho* >, allocator<CTCMapwho* > >::iterator, bool> _Rb_tree<CTCMapwho*, CTCMapwho*, _Identity<CTCMapwho* >, less<CTCMapwho* >, allocator<CTCMapwho* > >::insert_unique(CTCMapwho* const&);
}
