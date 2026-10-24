// _STL::__partial_sort(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* middle, pair<unsigned long, C3DVector>* last, pair<unsigned long, C3DVector>*, CKeyPairCompareLess<unsigned long, C3DVector> comp) -- heap-select the smallest keys into [first, middle) and sort them
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B530, 153 bytes (+int3 pad to 160), `ret 0Ch`.
// Callers: 0x0076BB5C and 0x0076BEF6 (the introsort loop 0x0076BE30's depth-limit fallback).
// Callees: __make_heap 0x0076A480, __adjust_heap 0x00769D50 (the inlined __pop_heap), sort_heap 0x0076AF10 (all the
//   same instantiation, /Gr __fastcall).
// Shape: STLport `__partial_sort(first, middle, last, _Tp*, comp)`: the inline make_heap forwards to __make_heap
//   with two null type pointers (`push 0 ; push 0 ; push comp`); for every i in [middle, last) with
//   comp(*i, *first) the inlined `__pop_heap(first, middle, i, _Tp(*i), comp, _Distance*)` stores *first into *i
//   and sifts the saved value down from the root (`xor edx,edx`); VC holds the saved vector's X/Y on the x87 stack
//   across the `*i = *first` copy. The trailing sort_heap is an out-of-line call.
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

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00769D50

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x0076A480

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0076AF10

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort<SKeyedVector*, SKeyedVector, SKeyedVectorLess>(SKeyedVector*, SKeyedVector*, SKeyedVector*, SKeyedVector*, SKeyedVectorLess);

}