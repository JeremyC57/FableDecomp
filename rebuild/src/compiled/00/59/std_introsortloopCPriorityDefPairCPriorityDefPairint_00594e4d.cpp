// std::__introsort_loop<CPriorityDefPair*, CPriorityDefPair, int, CContextSensitivePriorityCompare>(first, last, T*, depth_limit, comp) -- quicksort the context-sensitive item pairs down to 16-element runs, falling back to a heap sort at the depth limit
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594E4D, 135 bytes, `ret 0Ch` (`/Gr` `__fastcall`: first in ecx, last in edx;
// the unused value-type pointer, the depth limit and the 4-byte comparator slot on the stack).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes), its out-of-line copy ctor
// pair(const pair&) @ 0x00592DFB building the by-value pivot `_Tp(__median(...))` in its argument slot; ordered by
// CContextSensitivePriorityCompare (0x005928A8, landed). Callees: partial_sort(first, middle, last, comp) @ 0x00594DE2
// (forwards to `__partial_sort` 0x00594C77), `__median(a, b, c, comp)` @ 0x00592FFB, `__unguarded_partition(first,
// last, pivot, comp)` @ 0x00594637 (all this chunk) and itself (the right half).
// Callers: the sort driver at 0x00594EFF and its own recursion at 0x00594EAF.
// Label: the catalog "GameString_ConstructRecursive" is wrong: the body is STLport 4.x `__introsort_loop` verbatim:
// `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last, comp); return; }
// --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last - first)/2),
// *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }`
// (`__stl_threshold` = 16: the byte distance `and 0FFFFFFF8h ; cmp 80h`).
// Shape: first in ebx, last in edi, cut in esi, the depth limit decremented in its stack slot; the comparator push
// before the depth test is shared by both arms. Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CContextSensitiveItemDef;

template <class T> class CDefPointer {
public:
    T* PDef;                                         // the reference-counted def
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& other);                         // 0x00592DFB
    ~pair();                                         // 0x005930F3
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
const int __stl_threshold = 16;

void __fastcall partial_sort(CPriorityDefPair* __first, CPriorityDefPair* __middle, CPriorityDefPair* __last,
                             CContextSensitivePriorityCompare __comp);                       // 0x00594DE2
const CPriorityDefPair& __fastcall __median(const CPriorityDefPair& __a, const CPriorityDefPair& __b,
                                            const CPriorityDefPair& __c,
                                            CContextSensitivePriorityCompare __comp);       // 0x00592FFB
CPriorityDefPair* __fastcall __unguarded_partition(CPriorityDefPair* __first, CPriorityDefPair* __last,
                                                   CPriorityDefPair __pivot,
                                                   CContextSensitivePriorityCompare __comp); // 0x00594637

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop(CPriorityDefPair*, CPriorityDefPair*, CPriorityDefPair*, int,
                                          CContextSensitivePriorityCompare);
}