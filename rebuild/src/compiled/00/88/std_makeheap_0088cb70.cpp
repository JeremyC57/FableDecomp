// std::__make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*) -- (std::pair<long, CAIStateGroupBase*>, CKeyPairCompareGreater<long, CAIStateGroupBase*>) STLport heapify: sift every parent down from (len - 2) / 2 to the root through the out-of-line __adjust_heap
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088CB70, 74 bytes (+ int3 padding), `ret 0Ch` (__fastcall: first in ecx, last
//   in edx, then the 4-byte empty comparator slot and the value-type / distance-type tag pointers on the stack).
// Identity: std::pair<long, CAIStateGroupBase*> under CKeyPairCompareGreater<long, CAIStateGroupBase*> -- proven; evidence: its callers are the landed __partial_sort 0x0088CFD0 and __linear_insert 0x0088CD30 of Ego_r.pdb CAIBrain::StateGroupList (CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater<long, CAIStateGroupBase*> >); FableWin.pdb names make_heap<pair<long, CAIStateGroupBase*>*, CKeyPairCompareGreater<long, CAIStateGroupBase*> >.
// Label: the manifest's "std::make_heap_8byte" is propagated across byte-identical bodies (the eight rows differ only in the
//   __adjust_heap call displacement); this is STLport 4.6 `__make_heap` over an 8-byte pair.
// Callee: std::__adjust_heap(first, holeIndex, len, _Tp value, comp) @ 0x0088CA20 (__fastcall: first in ecx, holeIndex in
//   edx; len, the by-value 8-byte pair and the comparator on the stack).
// Body: STLport 4.6 _heap.c verbatim: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2;
//   for (;;) { __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`
//   -- the element is rebuilt in the argument area by pair's user copy constructor (`sub esp,8 ; mov eax,esp`).
class CAIStateGroupBase;
namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                          // +0
    _T2 second;                                         // +4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

}
template <class _Key, class _Value> class CKeyPairCompareGreater {   // empty key comparator (CVectorMap)
public:
    bool operator()(const std::pair<_Key, _Value>& __a, const std::pair<_Key, _Value>& __b) const { return __b.first < __a.first; }
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>                   // 0x0088CA20
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
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
typedef std::pair<long, CAIStateGroupBase*> _Elem;
template void __fastcall __make_heap(_Elem*, _Elem*, CKeyPairCompareGreater<long, CAIStateGroupBase*>, _Elem*, int*);
}