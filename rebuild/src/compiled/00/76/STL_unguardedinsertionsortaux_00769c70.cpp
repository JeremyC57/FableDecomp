// std::__unguarded_insertion_sort_aux(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* last, pair<unsigned long, C3DVector>*, CKeyPairCompareLess<unsigned long, C3DVector> comp) -- unguarded linear insertion of every element of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769C70, 64 bytes, `ret 8`.
// Callers: the landed SGI sort 0x0076C000 (its inlined __final_insertion_sort) and 0x0076B4AC / 0x0076C059.
// Callee: __unguarded_linear_insert 0x00769420 (same instantiation, out of line).
// Shape: STLport `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)`, /Gr (__fastcall: first in ecx, last in
//   edx, the null value-type pointer and the comparator on the stack). The comparator is loaded once into ebx;
//   `_Tp(*i)` is built in the argument area by the pair's user copy constructor (`sub esp,10h ; mov eax,esp`).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;                                              // +0x00 sort key
    _T2 second;                                             // +0x04
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, C3DVector> SKeyedVector;                 // 16 bytes
typedef CKeyPairCompareLess<unsigned long, C3DVector> SKeyedVectorLess;

namespace std {

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00769420

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux<SKeyedVector*, SKeyedVector, SKeyedVectorLess>(SKeyedVector*, SKeyedVector*, SKeyedVector*, SKeyedVectorLess);

}