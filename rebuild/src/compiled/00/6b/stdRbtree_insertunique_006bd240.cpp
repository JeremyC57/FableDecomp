// std::_Rb_tree<CCharString, pair<const CCharString, C2DVector>, ..>::insert_unique(const value_type&) -- STLport 4.6 unique insert into the region exit-text offset map's tree
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@VC2DVector@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BD240, 254 bytes.
// Identity: STLport 4.6 _tree.c insert_unique over map<CCharString, C2DVector> (the CRegion exit-text offset maps); its out-of-line callees
//   are the same tree's private _M_insert 0x006BD140 (NF82: AAE, the STLport private member) and _Rb_tree_iterator::operator-- 0x006BCDF0.
// NF87: moved off the row-local CCharString / C2DVector / STLport tree models onto stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE,
//   as NF85 0x00696B70), so _M_insert binds its private spelling; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
namespace std {
template pair<_Rb_tree<CCharString, pair<CCharString const, C2DVector>, _Select1st<pair<CCharString const, C2DVector> >, less<CCharString>, allocator<pair<CCharString const, C2DVector> > >::iterator, bool> _Rb_tree<CCharString, pair<CCharString const, C2DVector>, _Select1st<pair<CCharString const, C2DVector> >, less<CCharString>, allocator<pair<CCharString const, C2DVector> > >::insert_unique(const pair<CCharString const, C2DVector>&);
}
