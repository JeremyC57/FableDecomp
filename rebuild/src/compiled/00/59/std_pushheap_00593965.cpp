// std::__push_heap(std::pair<long,CDefPointer<const CContextSensitiveItemDef> >* first, int hole, int top, std::pair<long,CDefPointer<const CContextSensitiveItemDef> > val, CContextSensitivePriorityCompare pred) -- percolate a context-sensitive item def up a heap ordered by its priority, then destroy the by-value pair
// Flags: /Gr
// Leaf: ??$__push_heap@PAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@HU12@VCContextSensitivePriorityCompare@@@std@@
// Retail: Fable.exe @ 0x00593965, 98 bytes, size-optimised region with an ebp frame (unaligned;
// `optimize("y", off)` + `("s", on)`). Callees: CContextSensitivePriorityCompare::operator()
// 0x005928A8, pair<long,CDefPointer<..> >::operator= 0x0059308A (out of line in this TU),
// pair<long,CDefPointer<..> >::~pair 0x005930F3.
// Names (agentNF20, 2026-11-11): STLport 4.6 std::__push_heap over the std::pair (U) of FableWin.pdb
//   ??1?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@QAE@XZ (0x005930F3; was the row-namespace N00593965 stand-in);
//   same TU and model as the landed __unguarded_linear_insert 0x00593924.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, unrotated: `jmp` to the test) over an
// 8-byte pair (long key +0, CDefPointer +4). The predicate is a real (out-of-line) member functor
// called with `this` = the by-value comparator slot ([ebp+14h]) and (first[idx], val) pushed right
// to left; the pair's assignment and destructor are separate functions here.
// /Gr: first=ecx, hole=edx; `ret 10h` = top + 8-byte pair + the 4-byte comparator slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
class CContextSensitiveItemDef;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair& operator=(const pair& other);              // 0x0059308A
    ~pair();                                         // 0x005930F3 (FableWin.pdb ??1?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@QAE@XZ)
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    for (_Distance __parent = (__holeIndex - 1) / 2; __holeIndex > __topIndex && __comp(*(__first + __parent), __val); __parent = (__holeIndex - 1) / 2) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
    }
    *(__first + __holeIndex) = __val;
}

template void __push_heap<CPriorityDefPair*, int, CPriorityDefPair, CContextSensitivePriorityCompare>(CPriorityDefPair*, int, int, CPriorityDefPair, CContextSensitivePriorityCompare);
}
