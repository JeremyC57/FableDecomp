// _STL::__unguarded_partition(pair<ETCInterfaceType, CTCBase*>* first, pair<ETCInterfaceType, CTCBase*>* last, pair<ETCInterfaceType, CTCBase*> pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of a thing's component map around a by-value pivot entry; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CC690, 85 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, last in edx, the 8-byte pivot and the comparator slot on the stack). No callees.
// Caller: the matching __introsort_loop 0x004CD500 (this chunk).
// Identity: CVectorMap<ETCInterfaceType, CTCBase*> entry pair (8 bytes, signed key); the catalog's
//   "std::heapsort_SiftDown" is descriptive.
// Shape: STLport 4.x `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot key is held in ecx; the inlined
//   __iter_swap temporary lives in esi/edi (the pair's copy constructor, the implicit assignment).
enum ETCInterfaceType { TC_NONE = 0 };                  // the thing-component interface type (a signed key)
class CTCBase;

namespace _STL {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<ETCInterfaceType, CTCBase*> TComponentPair;          // CThing::Components entry, 8 bytes
typedef CKeyPairCompareLess<ETCInterfaceType, CTCBase*> TComponentLess;

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
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
template TComponentPair* __fastcall __unguarded_partition<TComponentPair*, TComponentPair, TComponentLess>(TComponentPair*, TComponentPair*, TComponentPair, TComponentLess);
}