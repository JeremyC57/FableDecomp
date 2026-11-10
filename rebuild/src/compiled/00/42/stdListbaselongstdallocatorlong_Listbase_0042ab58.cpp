// std::_List_base<long, std::allocator<long> >::_List_base(const std::allocator<long>&) -- STLport list base constructor: allocate the sentinel node and link it to itself (an empty list<long>)
// Leaf: ??0?$_List_base@JV?$allocator@J@std@@@std@@QAE@ABV?$allocator@J@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AB58, 27 bytes, `ret 4`, size-optimised TU.
// Identity (Codex name fix 2026-11-10): STLport 4.6 _list.h `_List_base(const allocator_type& __a) : _M_node(__a, (_Node*)0)
//   { _M_node._M_data = _M_node.allocate(1); _M_node._M_data->_M_next = _M_node._M_data; ... }`: the proxy's pointer zeroed
//   (`and dword ptr [esi],0`), malloc(0xc) for one _List_node<long>, next = prev = node. T = long: the callers hold list<long>
//   members by Ego_r.pdb layout (NUISystem::CAction / CManager ThingActionParams, CTCInventoryTrade::NotInStock,
//   CDrawQuestInfo::OracleSymbols, the four SExpSpendUndo histories), and the landed destructor of the same instance is
//   ~_List_base 0x0042AB73. Retail calls this base constructor out of line from the inline list(const allocator&).
//   Earlier landed as a free `__fastcall list` stand-in.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
template std::_List_base<long, std::allocator<long> >::_List_base(const std::allocator<long>&);
