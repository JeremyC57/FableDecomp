// std::_Rb_tree<CCharString, pair<CCharString const, CCountedPointer<CConsoleInputBase> >, _Select1st<pair<CCharString const, CCountedPointer<CConsoleInputBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<CConsoleInputBase> > > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EF750, 259 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<CCharString, CCountedPointer<CConsoleInputBase>> tree; Ego_r.pdb members of this type:
//   CConsole::RegisteredConsoleInputs. Landed callers of this address: 0x009EFB80 CacheTree::std::_Rb_tree::insert_unique.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CCacheInfo::InsertNode`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CConsoleInputBase;
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, CCountedPointer<CConsoleInputBase> >, _Select1st<pair<CCharString const, CCountedPointer<CConsoleInputBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<CConsoleInputBase> > > >::iterator _Rb_tree<CCharString, pair<CCharString const, CCountedPointer<CConsoleInputBase> >, _Select1st<pair<CCharString const, CCountedPointer<CConsoleInputBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<CConsoleInputBase> > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, pair<CCharString const, CCountedPointer<CConsoleInputBase> > const&, _Rb_tree_node_base*);
}
