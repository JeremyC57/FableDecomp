// std::__linear_insert(pair<long, CLongMapValue8>* first, pair<long, CLongMapValue8>* last, pair<long, CLongMapValue8> val, CKeyPairCompareLess comp) -- STLport insertion step: an entry smaller than the first shifts the whole run up one slot and goes first, otherwise it is sifted down unguarded
// Leaf: ??$__linear_insert@PAU?$pair@JVCLongMapValue8@@
// Retail 0x0086FA00, 0x5B bytes (+ int3 padding to 0x60), `ret 10h` (/Gr: first in ecx, last in edx, the 12-byte value
// and the empty comparator on the stack). No direct caller (the long-keyed map's __insertion_sort reaches it).
// Callees (same-TU stand-ins below, each out of line in retail): __copy_backward<pair<...>*, pair<...>*, int>(first, last,
// result, const random_access_iterator_tag&, int*) @ 0x0086EFF0 (/Gr; the tag homed on the dead comparator slot, the null
// distance pointer pushed) and __unguarded_linear_insert<pair<...>*, pair<...>, CKeyPairCompareLess<...> > @ 0x0086F090
// (last in ecx, the value copied into the argument block, then the comparator).
// Element: pair<long, 8-byte value> (0x0C; signed key `jge`), the map of 0x0086F3E0 / 0x0086F600 whose value type is not
// identified (CLongMapValue8 is a descriptive name). Byte twin of 0x0086C5C0 (the unsigned-key texture-morph map).
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
// __unguarded_linear_insert(last, val, comp);` with copy_backward -> __copy_backward_aux -> __copy_backward_ptrs
// (__false_type) inlined down to the random-access __copy_backward.
// Label: the manifest's "CPersistInfo_InsertionSortStep_0086fa00" is a descriptive auto label.
class CLongMapValue8                                 // 8 bytes of POD (descriptive)
{
public:
    long          Key;                               // +0x00
    unsigned long Data;                              // +0x04
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair
{
    _T1 first;                                       // +0x00
    _T2 second;                                      // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

#pragma auto_inline(off)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
    _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)                    // 0x0086EFF0
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
__declspec(noinline) void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)   // 0x0086F090
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
#pragma auto_inline()

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp>
__forceinline _Tp* copy_backward(_Tp* __first, _Tp* __last, _Tp* __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

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
}

template <class K, class V> class CKeyPairCompareLess
{
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<long, CLongMapValue8> TLongMapPair;

template void __fastcall std::__linear_insert<TLongMapPair*, TLongMapPair,
    CKeyPairCompareLess<long, CLongMapValue8> >(TLongMapPair*, TLongMapPair*,
    TLongMapPair, CKeyPairCompareLess<long, CLongMapValue8>);