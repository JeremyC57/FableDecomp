// _STL::__unguarded_insertion_sort_aux(float* first, float* last, float*, less<float> comp) -- unguarded insertion-sort pass over a float range: each value is sifted down past every larger predecessor with no lower-bound test
// Leaf: ??$__unguarded_insertion_sort_aux@PAMMU?$less@M@_STL@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A540, 50 bytes (+14 int3), `ret 8` (link-time __fastcall: first in ecx,
//   last in edx, the value-type tag pointer and the empty less<float> on the stack). No callees.
// Siblings in the same float-sort TU: __linear_insert 0x00A5A770, __insertion_sort 0x00A5A860, __partial_sort 0x00A5A8A0,
//   __adjust_heap 0x00A5A580, __make_heap 0x00A5A660, sort_heap 0x00A5A7D0.
// Label: the manifest's `_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>` is a propagation; the body is STLport 4.6's
//   `__unguarded_insertion_sort_aux<float*, float, less<float> >`.
// Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the linear insert and less<float>
//   inlined: the by-value float stays on the x87 stack (`fld [ecx]` ... `fstp [edi]`), `fcom [esi] ; test ah,5 ; jp` is
//   `val < *next`, the shifted element is copied as a dword.
namespace _STL {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template <class RandomAccessIter, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}

template void __unguarded_insertion_sort_aux(float*, float*, float*, less<float>);

}   // namespace _STL