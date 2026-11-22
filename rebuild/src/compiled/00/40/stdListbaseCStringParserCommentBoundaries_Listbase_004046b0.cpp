// std::_List_base<CStringParserCommentBoundaries>::~_List_base() -- STLport list base destructor: free every node, relink the sentinel to itself, then free the sentinel
// Leaf: ??1?$_List_base@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004046B0, 62 bytes, thiscall, plain `ret`. Callees: free 0x00BFEA14.
// Name fix (NF245, 2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): was `std::list<CStringParserCommentBoundaries>::~list()`
//   from a row-local list model (hand621 NAME_FIXES spelling). Two sources: (a) retail 0x00404710 is the size-optimised
//   list<CStringParserCommentBoundaries>::~list() COMDAT, a 5-byte `jmp 0x004046B0` (`e9 9b ff ff ff`, the only jmp to this address in the
//   exe; STLport 4.6 _list.h `~list() {}`, the clear + free in ~_List_base), the layout of 0x0063F360 / 0x0063F470 and 0x0042ABCA /
//   0x0042B6D2; (b) a shared-header TU compiles the four list<CStringParserCommentBoundaries> COMDATs equal to 0x00404690 (_List_base
//   ctor), 0x004046B0 (this), 0x004046F0 (list ctor) and 0x00404710 (~list) (pk.py, NF243 / NF245), and 18 landed CStringParser.h
//   rows already bind this address as ~_List_base. The PDBs carry a Dinkumware list (no _List_base), so the base spelling is STLport
//   4.6's: descriptive-by-library, as nf239 / nf241 / nf243 landed the list<CActionParam*> / list<CComponent*> / list<const
//   CThingGameObject*> destructors. The body is shared by ~90 byte-identical list destructors (COMDAT folding); the element type is
//   the comment list's (CStringParser::Comments), from engine/CStringParser.h's partial mode (no row-local element model).
// Shape: STLport 4.6 `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with clear() inlined: the loop variable
//   advances before the node is freed (esi), the sentinel is re-read from `this` (edi) after every call; the element is a POD
//   (nothing destroyed); the sentinel free is null-tested by the malloc allocator's deallocate.
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
template std::_List_base<CStringParserCommentBoundaries, std::allocator<CStringParserCommentBoundaries> >::~_List_base();
