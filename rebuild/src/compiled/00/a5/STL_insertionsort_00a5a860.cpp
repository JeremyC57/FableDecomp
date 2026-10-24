// _STL::__insertion_sort(float* first, float* last, less<float> comp) -- straight insertion sort of a float range: every element from first + 1 is passed by value to the out-of-line __linear_insert
// Leaf: ??$__insertion_sort@PAMU?$less@M@_STL@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A860, 50 bytes (+14 int3), `ret 4` (link-time __fastcall: first in ecx,
//   last in edx, the empty less<float> on the stack).
// Callees: __linear_insert<float*, float, less<float> > @ 0x00A5A770 (out of line, __fastcall, `ret 8`).
// Label: the manifest's `Vector_ForEach_ApplyFunc` is descriptive; the body is STLport 4.6's
//   `__insertion_sort<float*, less<float> >`.
// Shape: `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);` -- the
//   comparator's stack word is loaded once into ebp and pushed by value each pass, `*i` goes over as a dword.
namespace _STL {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class RandomAccessIter, class T, class Compare>
void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp);   // 0x00A5A770

template <class RandomAccessIter, class Compare>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

template void __insertion_sort(float*, float*, less<float>);

}   // namespace _STL