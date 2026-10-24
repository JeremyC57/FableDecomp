// _global::_Adjust_heap(std::pair<long,CDefPointer<const CContextSensitiveItemDef> >*, long, long, pair, CContextSensitivePriorityCompare) -- Dinkumware heap sift-down of (priority, context-sensitive item def pointer) pairs with an out-of-line priority comparator, then _Push_heap @ 0x00593965; destroys the by-value pair
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00593e3e, 125 bytes (`ret 10h`).
// Callees (all out of line in this size-optimised TU): CContextSensitivePriorityCompare::operator()
// 0x005928A8 (thiscall on the comparator's stack slot), pair::operator= 0x0059308A, the pair copy
// constructor 0x00592DFB (builds the by-value argument in its slot: `push ecx ; push ecx ; mov ecx,esp`),
// _Push_heap 0x00593965, ~pair 0x005930F3 (the callee destroys its by-value parameter).
// first in ecx, hole in edx (link-time __fastcall); bottom, the 8-byte pair and the comparator follow.
// Shape: the unaligned address, the `ebp` frame with `leave`, the `jmp` to the loop test and the
// out-of-line one-line helpers mark a /O1 TU: `#pragma optimize("s", on)` reproduces it.  `top` is
// the `[ebp-4]` local.  The element is 8 bytes (`[edi+esi*8]`): long priority + CDefPointer.
#pragma optimize("s", on)
namespace FableHeap_00593e3e {

class CContextSensitiveItemDef;

template<class T> struct CDefPointer {
    T* m_pDef;
};

template<class K, class V> struct pair {
    K first;
    V second;
    pair(const pair& other);                // 0x00592DFB
    pair& operator=(const pair& other);     // 0x0059308A
    ~pair();                                // 0x005930F3
};

typedef pair<long, CDefPointer<const CContextSensitiveItemDef> > SPriorityDef;

struct CContextSensitivePriorityCompare {
    char m_tag;
    bool operator()(const SPriorityDef& a, const SPriorityDef& b) const;   // 0x005928A8
};

void __fastcall _Push_heap(SPriorityDef* first, long hole, long top, SPriorityDef value, CContextSensitivePriorityCompare pred);   // 0x00593965

void __fastcall _Adjust_heap(SPriorityDef* first, long hole, long bottom, SPriorityDef value, CContextSensitivePriorityCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}