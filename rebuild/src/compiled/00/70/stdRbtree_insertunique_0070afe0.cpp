// std::_Rb_tree<long, pair<long const, CEngineTextureDecal>, _Select1st<pair<long const, CEngineTextureDecal> >, less<long >, allocator<pair<long const, CEngineTextureDecal> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with the instance's iterator operator-- (0x0070A330, out of line) unless at begin(), then _M_insert (0x0070AEA0) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJVCEngineTextureDecal@@@std@@U?$_Select1st@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJVCEngineTextureDecal@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBJVCEngineTextureDecal@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070AFE0, 180 bytes, thiscall `ret 8`; /O2 TU.
// Identity (NF58, round 7): the same tree as its callee _M_insert 0x0070AEA0 (see that row); its landed caller is CTCTextureDecal::PeekEngineDecals 0x0070A570.
// Was the stand-in `_unattributed::InsertUnique_CRetailTree_0070afe0` (local model); now the explicit instantiation of stlport/fable_map.h's insert_unique (FABLE_RETAIL_STLP_MAP
//   body) with the iterator's operator-- kept out of line (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE, as retail calls the per-instance
//   COMDAT 0x0070A330). STLport 4.6.2 source, descriptive-by-library.
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/CEngineTextureDecal.h"
namespace std {
template pair<_Rb_tree<long, pair<long const, CEngineTextureDecal>, _Select1st<pair<long const, CEngineTextureDecal> >, less<long >, allocator<pair<long const, CEngineTextureDecal> > >::iterator, bool> _Rb_tree<long, pair<long const, CEngineTextureDecal>, _Select1st<pair<long const, CEngineTextureDecal> >, less<long >, allocator<pair<long const, CEngineTextureDecal> > >::insert_unique(pair<long const, CEngineTextureDecal> const&);
}
