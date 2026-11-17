// std::_Rb_tree<CCharString, pair<const CCharString, vector<CCharString> >, _Select1st<..>, less<CCharString>, allocator<..> >::_M_erase(_Rb_tree_node* x) -- STLport recursive subtree release without rebalancing: for each node erase its right subtree, destroy the value (the string vector, then the key) and free the node, continuing with its left child
// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AE0E0, 110 bytes, thiscall, `ret 4`; recursive (calls itself at 0x007AE0F9).
// Callees: ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Callers: itself, the tree's clear 0x007AE190, CPersistContext::TransferMap 0x007AE3D0.., the tree destructor 0x007AEB70, an
//   _Erase_range at 0x008AE380, and the atexit destructors of CTCAIScratchpad::Gossip (0x01229CA0, landed NF57: binds this spelling).
// Shape: STLport `_M_erase(x) { while (x != 0) { _M_erase(_S_right(x)); _Link_type y = _S_left(x); _Destroy(&x->_M_value_field);
//   _M_header.deallocate(x, 1); x = y; } }`; the value is a pair<const CCharString, vector<CCharString> > (key +0x10, vector
//   +0x14..+0x1C) whose inline ~vector destroys [start, finish) and frees the null-tested start, then the key string.
// Name fix (NF59, queue q10550; NF57 q10430): the landed row spelled a local two-parameter _Rb_tree<K, V> with a class pair
//   (V?$pair); the shared stlport/fable_map.h tree (struct pair U?$pair, _Select1st, less, allocator; STLport 4.6) emits the member
//   that 0x01229CA0 binds. The local CCharString / std models are replaced by fable_string.h (FABLE_RETAIL_CCHARSTRING) and the
//   STLport headers; the explicit instantiation names the private member through map<..>::_Rep_type.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

typedef std::map<CCharString, std::vector<CCharString> > TGossipMap;
template void TGossipMap::_Rep_type::_M_erase(std::_Rb_tree_node<TGossipMap::value_type>* x);
