// std::_Rb_tree<CUnprovenKey_0083ADA0, CUnprovenValue_0083ADA0, std::_Select1st<CUnprovenValue_0083ADA0>, std::less<CUnprovenKey_0083ADA0>, std::allocator<CUnprovenValue_0083ADA0>>::_S_value(_Rb_tree_node<CUnprovenValue_0083ADA0>* __x) -- the value field of a node (static, _STLP_CALL = __cdecl)
// Leaf: ?_S_value@?$_Rb_tree@UCUnprovenKey_0083ADA0@@U?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@std@@U?$_Select1st@U?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@std@@@3@U?$less@UCUnprovenKey_0083ADA0@@@3@V?$allocator@U?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@std@@@3@@std@@KAAAU?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@2@PAU?$_Rb_tree_node@U?$pair@$$CBUCUnprovenKey_0083ADA0@@UCUnprovenData_0083ADA0@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083AE10, 8 bytes, cdecl: mov eax,[esp+4] ; add eax,10h ; ret.
// Identity (descriptive, unproven instantiation): one of eight unreferenced STLport 4.6 red-black-tree helpers 0x0083ADA0..0x0083AE20
//   (stl/_tree.h text; the node's value field at +0x10 after _M_color / _M_parent / _M_left / _M_right) kept as COMDATs of the
//   TU that also holds the landed _Rb_tree<CUnprovenKey_0083b120,...>::find 0x0083B120 and CRetailTreeIterator::Decrement
//   0x0083AEA0 (retail links without /OPT:REF). No call, jump or absolute reference to any of them exists (xref.py), so no
//   caller fixes the key/mapped types: the run shares the placeholders CUnprovenKey/Data/Value/Tree_0083ADA0 (a map, as the
//   _Select1st member 0x0083AE20 implies), and the member choice among byte-identical candidates is noted per row.
// Shape: explicit instantiation emits the inline member as its own COMDAT.
// Member: the static _S_value(_Link_type) (`return __x->_M_value_field`); _S_key (`_Select1st()(_S_value(x))`, the key
//   at offset 0 of the pair) has the same bytes.
// Model: the shared STLport 4.6 map (stlport/fable_map.h) with the run's placeholder key / data types.
#define FABLE_RETAIL_STLP_TREE_S_VALUE_CDECL
#include "stlport/fable_map.h"
struct CUnprovenKey_0083ADA0 { long _M_unproven; };
inline bool operator<(const CUnprovenKey_0083ADA0& __x, const CUnprovenKey_0083ADA0& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenData_0083ADA0 { long _M_unproven; };
typedef std::pair<const CUnprovenKey_0083ADA0, CUnprovenData_0083ADA0> CUnprovenValue_0083ADA0;
template class std::_Rb_tree<CUnprovenKey_0083ADA0, CUnprovenValue_0083ADA0, std::_Select1st<CUnprovenValue_0083ADA0>,
                             std::less<CUnprovenKey_0083ADA0>, std::allocator<CUnprovenValue_0083ADA0> >;
