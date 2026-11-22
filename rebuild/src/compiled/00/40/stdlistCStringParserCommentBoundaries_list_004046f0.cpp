// std::list<CStringParserCommentBoundaries>::list(const allocator&) -- STLport list constructor (the _List_base constructor inlined): allocate the 16-byte sentinel node and link it to itself
// Leaf: ??0?$list@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAE@ABV?$allocator@VCStringParserCommentBoundaries@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004046F0, 32 bytes, thiscall, `ret 4`. Callees: malloc 0x00BFEA0E.
// Name fix (NF245, 2026-11-22, the 0x004046B0 family; nf243 ruling 3): was the made-up free function
//   `void** __fastcall ListNode_Create_Circular(void**, void*, void*)` (catalog `_global::ListNode_Create_Circular`). No landed row and no
//   retail call or absolute reference reaches it (an unreferenced COMDAT). ONE discriminating source (coordinator decision, NF245
//   NOTES): its place in the list<CStringParserCommentBoundaries> COMDAT run, STLport instantiation order (_List_base ctor 0x00404690,
//   ~_List_base 0x004046B0, then this, then ~list 0x00404710 = `jmp 0x004046B0`), the slot list's own constructor takes in the run of
//   another list (list<const CThingAICreature*>: ctor 0x006860C0, bound so by the landed 0x00682810, just before its ~list thunk
//   0x006860E0). Its bytes are 0x00404690's (STLport `explicit list(const allocator_type& a = allocator_type()) : _List_base(a) {}`
//   with the base constructor inlined), so the body cannot tell the two constructors apart; a shared-header TU compiles this copy
//   equal to it (pk.py). Dinkumware PDBs: the STLport spelling is descriptive-by-library and, here, descriptive-by-layout.
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
template std::list<CStringParserCommentBoundaries, std::allocator<CStringParserCommentBoundaries> >::list(const std::allocator<CStringParserCommentBoundaries>&);
