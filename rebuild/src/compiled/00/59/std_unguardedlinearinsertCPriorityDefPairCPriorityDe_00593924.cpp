// std::__unguarded_linear_insert<CPriorityDefPair*, CPriorityDefPair, CContextSensitivePriorityCompare>(last, val, comp) -- shift higher-priority neighbours up until val's slot is found (insertion-sort inner loop), then destroy the by-value val
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00593924, 65 bytes, `ret 0Ch` (`/Gr` `__fastcall`: last in ecx; the 8-byte
// by-value pair and the 4-byte comparator slot on the stack).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes) with its out-of-line assignment
// pair::operator= @ 0x0059308A and destructor ~pair @ 0x005930F3 (the CDefPointer release; the callee destroys its
// by-value parameter), ordered by CContextSensitivePriorityCompare::operator() const @ 0x005928A8 (landed; thiscall
// on the comparator's own stack slot `lea ecx,[ebp+10h]`). Same TU and model as the landed STLport `__push_heap`
// 0x00593965 and `__adjust_heap` 0x00593E3E.
// Callers: `__linear_insert` @ 0x00594946 (this chunk; the copy at 0x00594992) and the `__unguarded_insertion_sort`
// loop at 0x00593E28.
// Label: the catalog "PriorityQueue_SortedInsert" is descriptive: the body is STLport 4.x
// `__unguarded_linear_insert` verbatim: `next = last; --next; while (comp(val, *next)) { *last = *next; last = next;
// --next; } *last = val;`.
// Shape: last in edi, next in esi (`lea esi,[edi-8]`), the loop entered at its comparator test; every assignment is
// `mov ecx,<dest> ; push <src> ; call operator=`. Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CContextSensitiveItemDef;

template <class T> class CDefPointer {
public:
    T* PDef;                                         // the reference-counted def (released by ~pair 0x005930F3)
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair& operator=(const pair& other);              // 0x0059308A
    ~pair();                                         // 0x005930F3
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template void __fastcall __unguarded_linear_insert(CPriorityDefPair*, CPriorityDefPair, CContextSensitivePriorityCompare);
}