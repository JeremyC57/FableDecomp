// std::_List_base<CStringParserCommentBoundaries>::_List_base(const allocator&) -- STLport list base constructor: allocate the 16-byte sentinel node and link it to itself (the empty list)
// Leaf: ??0?$_List_base@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAE@ABV?$allocator@VCStringParserCommentBoundaries@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404690, 32 bytes, thiscall, `ret 4`. Callees: malloc 0x00BFEA0E.
// Name fix (NF245, 2026-11-22, the 0x004046B0 family; nf243 ruling 3): was the made-up free function
//   `void** __fastcall std_list_node_Init(void**, void*, void*)` (catalog `_global::std_list_node_Init`). Two sources: (a) its only
//   landed caller, the list<CStringParserCommentBoundaries> copy constructor 0x00463871 (STLport 4.6 `list(const list& x) :
//   _List_base(x.get_allocator())`), calls it at 0x0046387C and binds this spelling; (b) it opens the
//   list<CStringParserCommentBoundaries> COMDAT run in STLport instantiation order: _List_base ctor 0x00404690, ~_List_base 0x004046B0,
//   list(const allocator&) 0x004046F0 (the same body, base ctor inlined), ~list 0x00404710 (`jmp 0x004046B0`), the layout of
//   0x0042AB58 / 0x0042AB73 (list<long>, Codex 2026-11-10); a shared-header TU compiles this copy equal to 0x00404690 (pk.py). The
//   PDBs carry a Dinkumware list, so the spelling is STLport 4.6's: descriptive-by-library, like the landed ~_List_base rows.
// Shape: `_List_base(const allocator_type&) : _M_node(allocator<_Node>(), 0) { _M_node._M_data = allocate(1); next = prev = node; }`:
//   the head word cleared first (alloc_proxy's member init), malloc(0x10), both links to the node, the node stored.
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
template std::_List_base<CStringParserCommentBoundaries, std::allocator<CStringParserCommentBoundaries> >::_List_base(const std::allocator<CStringParserCommentBoundaries>&);
