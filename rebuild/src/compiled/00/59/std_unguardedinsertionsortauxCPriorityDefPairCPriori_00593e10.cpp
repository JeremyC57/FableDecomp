// std::__unguarded_insertion_sort_aux<CPriorityDefPair*, CPriorityDefPair, CContextSensitivePriorityCompare>(first, last, T*, comp) -- finish the introsort with unguarded insertion steps over [first, last)
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x00593E10, 41 bytes, `ret 8` (`/Gr` `__fastcall`: first in ecx, last in edx; the
// unused value-type pointer and the 4-byte comparator slot on the stack).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes), its out-of-line copy ctor
// pair(const pair&) @ 0x00592DFB builds each by-value `_Tp(*__i)` argument in its stack slot (`push ecx ; push ecx ;
// mov ecx,esp ; push esi ; call`); callee `__unguarded_linear_insert(i, val, comp)` @ 0x00593924 (this chunk) destroys it.
// Label: the catalog "_Fill<CIntelligentPointer<CThing>*,...>" is a propagated Dinkumware spelling: the body is STLport
// 4.x `__unguarded_insertion_sort_aux` verbatim: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i),
// comp);` (the comparator slot pushed first, the loop entered at its `cmp esi,edi` bottom test).
// Shape: size TU (`#pragma optimize("s", on)`), frameless.
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
void __fastcall __unguarded_linear_insert(CPriorityDefPair* __last, CPriorityDefPair __val,
                                          CContextSensitivePriorityCompare __comp);           // 0x00593924

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(CPriorityDefPair*, CPriorityDefPair*, CPriorityDefPair*,
                                                        CContextSensitivePriorityCompare);
}