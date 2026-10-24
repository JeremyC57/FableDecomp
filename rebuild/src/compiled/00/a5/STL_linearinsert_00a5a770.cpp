// _STL::__linear_insert(float* first, float* last, float val, less<float> comp) -- insertion-sort step: a value below the first element memmoves [first, last) up one slot and goes first, anything else sifts down from last
// Leaf: ??$__linear_insert@PAMMU?$less@M@_STL@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A770, 87 bytes (+9 int3), `ret 8` (link-time __fastcall: first in ecx,
//   last in edx, the float value and the empty less<float> on the stack).
// Callees: memmove @ 0x00BFEAE6 (STLport's trivial copy_backward for float*). Caller: __insertion_sort @ 0x00A5A860.
// Label: the manifest's `Array_InsertionSort_Float` is descriptive; the body is STLport 4.6's
//   `__linear_insert<float*, float, less<float> >` (inline, emitted out of line).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward over float* is STLport's __copy_trivial_backward
//   (`num = (char*)last - (char*)first; num > 0 ? memmove((char*)result - num, first, num) : result`), the float
//   stores are dword moves, the unguarded insert reloads `val` from its slot for every compare.
#include <stddef.h>
#include <string.h>

namespace _STL {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

inline void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const ptrdiff_t num = (const char*)last - (const char*)first;
    return (num > 0) ? memmove((char*)result - num, first, num) : result;
}

template <class T>
inline T* copy_backward(T* first, T* last, T* result)
{
    return (T*)__copy_trivial_backward(first, last, result);
}

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
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template void __linear_insert(float*, float*, float, less<float>);

}   // namespace _STL