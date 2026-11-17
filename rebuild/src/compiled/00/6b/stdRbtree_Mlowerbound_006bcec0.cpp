// std::_Rb_tree<CCharString, std::pair<CCharString const, C2DVector>, ...>::_M_lower_bound(const CCharString& k) const -- STLport 4.6 lower-bound walk: the last node whose key is not less than k (the header when none)
// Leaf: ?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VC2DVector@@@std@@@3@@std@@ABEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BCEC0, 65 bytes, thiscall `ret 4`; returns the node pointer (the 0x004B9D00 / 0x009AADB0 shape).
// Identity: Ego_r.pdb CRegion::MiniMapRegionExitTextOffsets is map<CCharString, C2DVector>; the landed operator[] 0x006BD6A0 (its only caller, map<CCharString, C2DVector> per its own identity note) binds this address as that map's _M_lower_bound.
// Callees: CBasicString<char>::operator< 0x00429950 (through the inline CCharString::operator<).
// NF82 (q11007): re-authored from the extern "C" Tree_Lbound stand-in (a local CCharString with a `Less` member that bound 0x00429950 as
//   ?Less@CCharString@@QAE_NPAV1@@Z, and local node / tree models; catalog std::_Tree<std::_Tmap_traits<CCharString,float,..>>::_Lbound,
//   a Dinkumware stand-in) as the explicit instantiation of the private STLport _M_lower_bound on stlport/fable_map.h
//   (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE + _BODY_PASS22, the 0x004B9D00 / 0x009AADB0 recipe) and fable_string.h
//   (FABLE_RETAIL_CBASICSTRING_LESS_BODY): the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#define FABLE_RETAIL_C2DVECTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
namespace std {
template _Rb_tree_node_base* std::_Rb_tree<CCharString, pair<CCharString const, C2DVector >, _Select1st<pair<CCharString const, C2DVector > >, less<CCharString>, allocator<pair<CCharString const, C2DVector > > >::_M_lower_bound(const CCharString&) const;
}
