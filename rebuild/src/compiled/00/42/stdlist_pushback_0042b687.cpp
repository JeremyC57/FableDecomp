// std::list<long>::push_back(const long& value) -- STLport 4.6 _list.h push_back: insert(end(), value) through the out-of-line insert @ 0x0042AB33
// Leaf: ?push_back@?$list@JV?$allocator@J@std@@@std@@QAEXABJ@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042B687, 26 bytes (`ret 4`). Label correction (chunk 345 report): the
// manifest's "LinkedList_PushFront" is wrong -- insert(end(), x) links the new node BEFORE the sentinel, i.e. at the END.
// Twins (immediates masked): 0x0042B6B8 "Std_List_PushFront", 0x0042B646, 0x00493D16, 0x0053CAE8 .. (other element types).
// Callees: list<long>::insert(iterator position, const long& value) @ 0x0042AB33 (`ret 0Ch`: hidden iterator result slot,
// the by-value iterator and the value reference; it mallocs the 12-byte node via 0x00429FE3 and links it before position).
// Identity (NF95, 2026-11-19, queue q11225): Ego_r.pdb + FableWin.pdb spell the allocator a class (`V?$allocator@J@std@@`); the
// landed callers 0x005A30A1 / 0x00655ECE bind list<long> so. Was a row-local STLport model with `struct allocator` (U); now an
// explicit instantiation over stlport/fable_containers.h, the 0x0042B6B8 recipe (FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE +
// FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE), so insert binds the list<long> spelling 0x00655ECE uses. Descriptive-by-library.
// Shape: frame-pointer TU (`push ebp ; mov ebp,esp`), the end() iterator is built straight into its push slot
// (`push ecx ; mov eax,esp ; mov [eax],edx`, STLport's user copy constructor) and the dead `value` argument slot receives
// the discarded hidden iterator result (`lea eax,[ebp+8] ; push eax`); the reference itself is forwarded (`push [ebp+8]`).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "stlport/fable_containers.h"
template void std::list<long>::push_back(const long&);
