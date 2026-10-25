// _STL::__linear_insert(pair<ETCInterfaceType, CTCBase*>* first, pair<ETCInterfaceType, CTCBase*>* last, pair<ETCInterfaceType, CTCBase*> val, CKeyPairCompareLess comp) -- STLport insertion-sort step over a thing's component map: a new smallest interface type shifts the run up one slot and goes first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD0C0, 99 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, last in edx, the 8-byte value and the comparator slot on the stack).
// Caller: 0x004CD35B (the landed __insertion_sort 0x004CD330). Callee: __unguarded_linear_insert 0x004CB5E0 (this chunk).
// Identity: CVectorMap<ETCInterfaceType, CTCBase*> entry pair (8 bytes, signed key); see 0x004CB5E0.
// Shape: STLport 4.x verbatim. copy_backward of a pair with a user copy constructor (not trivially assignable) goes
//   through __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0) = the counted `for (n = last -
//   first; n > 0; --n) *--result = *--last;` loop, inlined; the else arm copy-constructs val in the argument area.
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x004CB5E0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template void __fastcall __linear_insert<TComponentPair*, TComponentPair, TComponentLess>(TComponentPair*, TComponentPair*, TComponentPair, TComponentLess);
}