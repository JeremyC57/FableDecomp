// std::__unguarded_partition<CPriorityDefPair*, CPriorityDefPair, CContextSensitivePriorityCompare>(first, last, pivot, comp) -- Hoare partition of the context-sensitive item pairs around a by-value pivot
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594637, 82 bytes, `ret 0Ch` (`/Gr` `__fastcall`: first in ecx, last in edx; the
// 8-byte by-value pivot and the 4-byte comparator slot on the stack; returns the split point).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes), ordered by
// CContextSensitivePriorityCompare::operator() const @ 0x005928A8 (landed; thiscall on the comparator slot
// `lea ecx,[ebp+10h]`); iter_swap(a, b) @ 0x005938F1 (`__fastcall`, the inline `__iter_swap` temporary); the
// by-value pivot is destroyed by ~pair @ 0x005930F3 (the CDefPointer release) before the return.
// Only caller: the STLport `__introsort_loop` @ 0x00594E4D (this chunk).
// Label: the catalog "Quicksort_Partition_CContextSensitive" is descriptive: the body is STLport 4.x
// `__unguarded_partition` verbatim: `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot,
// *last)) --last; if (!(first < last)) return first; iter_swap(first, last); ++first; }`.
// Shape: first in esi, last in edi, the outer loop entered past its trailing `++first` (`jmp`), the second scan a
// bottom-tested `--last` loop. Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
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
    ~pair();                                         // 0x005930F3
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
void __fastcall iter_swap(CPriorityDefPair* __i1, CPriorityDefPair* __i2);   // 0x005938F1

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template CPriorityDefPair* __fastcall __unguarded_partition(CPriorityDefPair*, CPriorityDefPair*, CPriorityDefPair,
                                                            CContextSensitivePriorityCompare);
}