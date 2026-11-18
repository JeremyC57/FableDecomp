// std::_Rb_tree<CCharString, std::pair<CCharString const, CBankFileManager::CBankInfo>, ...>::_M_lower_bound(const CCharString& k) const -- STLport 4.6 lower-bound walk of map<CCharString, CBankFileManager::CBankInfo> (Ego_r.pdb CBankFileManager::Banks): the last node whose key is not less than k (the header when none)
// Leaf: ?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@3@@std@@ABEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AADB0, 65 bytes, thiscall `ret 4`; returns the node pointer.
// Identity: NF80's 0x009AC5D0 (map<CCharString, CBankFileManager::CBankInfo>::operator[], Ego_r.pdb CBankFileManager::Banks; RM) calls
//   0x009AADB0 as this tree's private _M_lower_bound (fable_map.h FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE). The catalog name
//   std::_Tree<std::_Tmap_traits<CCharString,float,..>>::_Lbound was a Dinkumware spelling (retail is STLport 4.6).
// Callees: CBasicString<char>::operator< 0x00429950 (through the inline CCharString::operator<: equal data pointers -> false, null
//   node key -> true, null k -> false).
// Shape: STLport 4.6 _tree.c `_M_lower_bound` (fable_map.h FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22), the 0x004B9D00 recipe.
// NF82 (q10995 / q11007): re-authored from the extern "C" Tree_Lbound stand-in (local CCharString with a `Less` member, local node /
//   tree models) onto stlport/fable_map.h (explicit instantiation), fable_retail_banks.h (CBankFileManager::CBankInfo) and
//   fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY); the string compare now binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z
//   (was ?Less@CCharString@@QAE_NPAV1@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "fable_retail_banks.h"
namespace std {
template _Rb_tree_node_base* _Rb_tree<CCharString, pair<CCharString const, CBankFileManager::CBankInfo>, _Select1st<pair<CCharString const, CBankFileManager::CBankInfo> >, less<CCharString>, allocator<pair<CCharString const, CBankFileManager::CBankInfo> > >::_M_lower_bound(const CCharString&) const;
}
