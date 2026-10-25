// std::__copy_backward<pair<unsigned __int64, list<C2DLineF> >*, pair<unsigned __int64, list<C2DLineF> >*, int>(first,
// last, result, const random_access_iterator_tag&, int*) -- STLport random-access copy_backward over the 16-byte
// CNavigationLayer::SwitchableLineLists entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A821C0, length 70 bytes (+ int3 padding), `ret 0Ch`.
// Callees: list<C2DLineF>::operator= @ 0x00517720 (catalog "std::list::operator=").
// Shape: STLport `__copy_backward(first, last, result, random_access_iterator_tag, _Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned. /Gr TU (__fastcall: first in ecx, last in edx,
// result + the tag reference + the distance pointer on the stack, `ret 0Ch`). The pair's implicit operator= is inlined
// memberwise: the two words of the 64-bit key, then the out-of-line list assignment of the member at +8; the count is
// `sar eax,4` (16-byte entries: 8-byte key, the one-word STLport list, 4 bytes of padding).
// Element: Ego_r.pdb CNavigationLayer::SwitchableLineLists = CVectorMap<unsigned __int64, list<C2DLineF>,
//   CKeyPairCompareLess<unsigned __int64, list<C2DLineF> > >, whose value_type is this pair (agent422 NOTES: retail
//   STLport lists are one word, so the landed "8-byte list {head, size}" model was wrong; list<C2DLineF> operator= /
//   destructor are 0x00517720 / 0x005127A0). Was catalogued `_global::CopyBackward_SPairWithList10_00a821c0` (and the
//   manifest's `_Copy_backward<CSharerParserOutputToDebugContextMapper::CEntry*>` is propagated).
// Leaf: ??$__copy_backward@PAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@PAU12@H@std@@YIPAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                   // 16 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                              // +0x00
    C2DVector Point2;                              // +0x08
};

namespace std {
template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                       // STLport 4.6: one word, the sentinel node pointer
public:
    void* _M_node;
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct random_access_iterator_tag { };

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

typedef pair<unsigned __int64, list<C2DLineF> > _CSwitchableLineEntry;
template _CSwitchableLineEntry* __fastcall __copy_backward(_CSwitchableLineEntry*, _CSwitchableLineEntry*, _CSwitchableLineEntry*,
                                                           const random_access_iterator_tag&, int*);
}
