// std::__adjust_heap(std::pair<long,CDefPointer<const CContextSensitiveItemDef> >*, long, long, pair, CContextSensitivePriorityCompare) -- Dinkumware heap sift-down of (priority, context-sensitive item def pointer) pairs with an out-of-line priority comparator, then _Push_heap @ 0x00593965; destroys the by-value pair
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@HU12@VCContextSensitivePriorityCompare@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00593e3e, 125 bytes (`ret 10h`).
// Callees (all out of line in this size-optimised TU): CContextSensitivePriorityCompare::operator()
// 0x005928A8 (thiscall on the comparator's stack slot), pair::operator= 0x0059308A, the pair copy
// constructor 0x00592DFB (builds the by-value argument in its slot: `push ecx ; push ecx ; mov ecx,esp`),
// _Push_heap 0x00593965, ~pair 0x005930F3 (the callee destroys its by-value parameter).
// first in ecx, hole in edx (link-time __fastcall); bottom, the 8-byte pair and the comparator follow.
// Shape: the unaligned address, the `ebp` frame with `leave`, the `jmp` to the loop test and the
// out-of-line one-line helpers mark a /O1 TU: `// Names (agentNF20, 2026-11-11): STLport 4.6 std::__adjust_heap (the sibling of __push_heap 0x00593965 and the landed
//   __unguarded_linear_insert 0x00593924) over the std::pair (U) whose dtor is FableWin.pdb
//   ??1?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@QAE@XZ 0x005930F3 (was the row-namespace FableHeap_00593e3e stand-in).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
class CContextSensitiveItemDef;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& other);                         // 0x00592DFB
    pair& operator=(const pair& other);              // 0x0059308A
    ~pair();                                         // 0x005930F3 (FableWin.pdb ??1?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@std@@QAE@XZ)
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    char m_tag;
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00593965

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    for (; __secondChild < __len; __secondChild = 2 * __secondChild + 2) {
        if (__comp(__first[__secondChild], __first[__secondChild - 1]))
            --__secondChild;
        __first[__holeIndex] = __first[__secondChild], __holeIndex = __secondChild;
    }
    if (__secondChild == __len) {
        __first[__holeIndex] = __first[__secondChild - 1];
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __adjust_heap<CPriorityDefPair*, int, CPriorityDefPair, CContextSensitivePriorityCompare>(CPriorityDefPair*, int, int, CPriorityDefPair, CContextSensitivePriorityCompare);
}
