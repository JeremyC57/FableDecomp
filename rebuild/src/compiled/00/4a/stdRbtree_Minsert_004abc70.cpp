// std::_Rb_tree<CCharString, CCharString, _Identity, less<CCharString> >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const CCharString&, _Rb_tree_node_base*) -- STLport red-black tree node insert for a set<CCharString>: link a new node as y's left child (y is the header, or x is set, or v < key(y), unless a hint w is given) else as its right child, fix the leftmost/rightmost/root links, rebalance, count it, return the iterator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ABC70, 206 bytes, `ret 14h` (__fastcall: this -> ebp; the hidden
//   iterator result, x, y, &v and the hint w on the stack).
// Callees: 0x00BFEA0E malloc (STLport malloc allocator: `allocate(1)` folds to malloc(0x14)), 0x0099EC30 CCharString copy
//   ctor (the value constructed at node + 0x10, address null-tested), 0x00429950 CBasicString<char>::operator< (the
//   string-data compare behind the inline CCharString::operator<), 0x0042971D _Rb_global<bool>::_Rebalance(x, root&)
//   (static, __cdecl).
// Callers: set<CCharString>::insert_unique 0x004AC380 (calls at 0x004AC3F6 / 0x004AC44F).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4, empty comparator +8 }; nodes =
//   _Rb_tree_node_base { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xC } + the CCharString value at +0x10
//   (0x14-byte nodes). The header's parent is the root, its left the leftmost, its right the rightmost node.
// Shape: STLport 4.x `_M_insert` verbatim (`if (y == header || (w == 0 && (x != 0 || comp(KeyOfValue()(v),
//   S_key(y))))) { z = create(v); left(y) = z; if (y == header) { root = z; rightmost = z; } else if (y ==
//   leftmost) leftmost = z; } else { z = create(v); right(y) = z; if (y == rightmost) rightmost = z; } parent(z) = y;
//   left(z) = 0; right(z) = 0; _Rebalance(z, header->_M_parent); ++_M_node_count; return iterator(z);`); VC7.1 lays the
//   right arm out first. CCharString::operator< and the CBasicString<char>::operator< body as the landed tree lookups
//   (0x007ACD30): equal data pointers -> false, null left -> true, null right -> false, else the data compare.
// Label correction: catalogued as `StdSet_InsertNode` (right idea); STLport `_Rb_tree::_M_insert` of set<CCharString>.
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@std@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@std@@AAE?AU?$_Rb_tree_iterator@VCCharString@@U?$_Nonconst_traits@VCCharString@@@std@@@2@PAU_Rb_tree_node_base@2@0ABVCCharString@@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<CCharString, CCharString, _Identity<CCharString >, less<CCharString >, allocator<CCharString > >::iterator _Rb_tree<CCharString, CCharString, _Identity<CCharString >, less<CCharString >, allocator<CCharString > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const CCharString&, _Rb_tree_node_base*);
}
