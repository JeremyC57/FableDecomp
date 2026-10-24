// std::__make_heap<CPriorityDefPair*, CContextSensitivePriorityCompare, CPriorityDefPair, int>(first, last, comp, T*, int*) -- heapify the context-sensitive item pairs by sifting every parent down
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594706, 72 bytes, `ret 0Ch` (`/Gr` `__fastcall`: first in ecx, last in edx; the
// comparator slot and the two unused type pointers on the stack).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes); its out-of-line copy ctor
// pair(const pair&) @ 0x00592DFB builds each by-value `_Tp(*(first + parent))` in its argument slot (`push ecx ;
// push ecx ; mov ecx,esp ; push ebp ; call`); callee `__adjust_heap(first, parent, len, value, comp)` @ 0x00593E3E
// (landed as `_Adjust_heap`).
// Label: the catalog "Heap_BuildFromRange" is descriptive: the body is STLport 4.x `__make_heap` verbatim:
// `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first, parent,
// len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`.
// Shape: len in edi (the `< 2` test reuses it), parent in esi, `first + parent` strength-reduced into ebp. Size TU:
// `#pragma optimize("s", on)`, frameless (ebp is a register).
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
void __fastcall __adjust_heap(CPriorityDefPair* __first, int __holeIndex, int __len, CPriorityDefPair __value,
                              CContextSensitivePriorityCompare __comp);                     // 0x00593E3E

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template void __fastcall __make_heap(CPriorityDefPair*, CPriorityDefPair*, CContextSensitivePriorityCompare,
                                     CPriorityDefPair*, int*);
}