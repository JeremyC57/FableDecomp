// _STL::__unguarded_partition(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* last, pair<unsigned long, C3DVector> pivot, CKeyPairCompareLess<unsigned long, C3DVector> comp) -- Hoare partition around pivot's key, return the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769AC0, 140 bytes, `ret 14h`.
// Caller: 0x0076BEB8 (__introsort_loop 0x0076BE30 of the same instantiation). No callees.
// Shape: STLport `__unguarded_partition(first, last, pivot, comp)`, /Gr (__fastcall: first in ecx, last in edx, the
//   16-byte pivot and the comparator on the stack): both scans are rotated loops on the UNSIGNED key, and
//   `iter_swap(first, last)` is inlined (`_Tp tmp = *a; *a = *b; *b = tmp;`). Only pivot.first is ever read, so VC
//   parks tmp's three vector words in the pivot's dead argument slots (`[esp+18h..20h]`).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

namespace _STL {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;                                              // +0x00 sort key
    _T2 second;                                             // +0x04
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, C3DVector> SKeyedVector;                 // 16 bytes
typedef CKeyPairCompareLess<unsigned long, C3DVector> SKeyedVectorLess;

namespace _STL {

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
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

template SKeyedVector* __fastcall __unguarded_partition<SKeyedVector*, SKeyedVector, SKeyedVectorLess>(SKeyedVector*, SKeyedVector*, SKeyedVector, SKeyedVectorLess);

}