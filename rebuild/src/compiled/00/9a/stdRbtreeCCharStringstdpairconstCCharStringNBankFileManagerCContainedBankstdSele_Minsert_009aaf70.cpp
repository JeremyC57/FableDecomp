// std::_Rb_tree<CCharString, std::pair<const CCharString, NBankFileManager::CContainedBank>, std::_Select1st<..>, std::less<CCharString>, std::allocator<..> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- NBankFileManager::CRetailBank::ContainedBanks (map<CCharString, CContainedBank>) node insert: create a node for v, link it left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AAF70, 173 bytes, thiscall `ret 14h` (the hidden iterator result pointer first).
// Identity (sweep3, family pass 1 "invented SRecord20"): _M_create_node 0x009AA9E0 copies the value as a CCharString
//   copy + five dwords: Ego_r.pdb's NBankFileManager::CContainedBank {DataVersion, EntryCount, InfoOffset, InfoSize,
//   Alignment} (0x14 bytes, all ulong), and CRetailBank::ContainedBanks is the only Ego_r member typed
//   map<CCharString, NBankFileManager::CContainedBank>; the unreferenced STLport _Construct 0x009AA520 beside it
//   copies the same pair. The old SRecord20 was an invented stand-in for CContainedBank.
// Callers: nine call sites 0x009AB096..0x009ABA73 (insert_unique and the hinted insert_unique of the same tree).
// Callees: CBasicString<char>::operator< @ 0x00429950 (CCharString::operator< inlined), _M_create_node @ 0x009AA9E0
//   (out of line: malloc(0x28) + the pair copy; RENAMES row this pass), _Rb_global<bool>::_Rebalance @ 0x0042971D.
// Shape: STLport 4.6 `_tree.c` `_M_insert` verbatim (right-insert arm first, left-insert arm second; with _M_create_node
//   out of line the zero links are immediate stores); access per STLport 4.6 `_tree.h` (sweep2 decision 1d).
// NF80 (q10923): moved off the local CBasicStringChar / CCharString / std tree models onto stlport/fable_map.h (the _tree.c _M_insert
//   body, FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, with FABLE_RETAIL_STLP_TREE_CREATE_NODE_NOINLINE for the out-of-line
//   _M_create_node 0x009AA9E0; the 0x009EF750 recipe), fable_retail_banks.h (FABLE_RETAIL_CBANKFILEMANAGER:
//   NBankFileManager::CContainedBank) and fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_CREATE_NODE_NOINLINE
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "fable_retail_banks.h"
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, NBankFileManager::CContainedBank>, _Select1st<pair<CCharString const, NBankFileManager::CContainedBank> >, less<CCharString >, allocator<pair<CCharString const, NBankFileManager::CContainedBank> > >::iterator _Rb_tree<CCharString, pair<CCharString const, NBankFileManager::CContainedBank>, _Select1st<pair<CCharString const, NBankFileManager::CContainedBank> >, less<CCharString >, allocator<pair<CCharString const, NBankFileManager::CContainedBank> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, pair<CCharString const, NBankFileManager::CContainedBank> const&, _Rb_tree_node_base*);
}
