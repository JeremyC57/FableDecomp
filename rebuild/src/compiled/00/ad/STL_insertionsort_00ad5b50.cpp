// _STL::__insertion_sort(SKeyPair8* first, SKeyPair8* last, PFNKeyPairLess comp) -- STLport insertion sort over 8-byte (long, long) pairs ordered by a comparison FUNCTION pointer: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD5B50, 54 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x00AD4880 _STL::__linear_insert(first, last, value, comp) (manifest "std::_InsertionSort_PartitionHelper" is
// propagated; its only caller is this body). That callee calls the comparator indirectly (`call ebx` with ecx = &value,
// edx = first), so the comparator here is a plain `bool (__fastcall*)(const T&, const T&)` function pointer.
// Label: catalogued as `_Insertion_sort1<pair<long,long>*, CKeyPairCompareLess<long,long> >` (Dinkumware name, propagated
// over byte-identical bodies); the code is STLport's `__insertion_sort`, which hands `*i` BY VALUE to __linear_insert.
// Shape: the element is a POD of two dwords, so the by-value copy is two plain pushes (`push [i+4] ; push [i]`), the
// comparator dword is loaded once into ebp and pushed each iteration. The callee-saved pushes are shrink-wrapped behind
// the two early exits (VC7.1 RTM does this on its own for this loop).
struct SKeyPair8 {                        // std::pair<long, long>
    long first;                           // +0x00 sort key
    long second;                          // +0x04
};

typedef bool (__fastcall* PFNKeyPairLess)(const SKeyPair8& a, const SKeyPair8& b);

void __fastcall __linear_insert(SKeyPair8* first, SKeyPair8* last, SKeyPair8 value, PFNKeyPairLess comp);   // 0x00AD4880

void __fastcall __insertion_sort(SKeyPair8* first, SKeyPair8* last, PFNKeyPairLess comp)
{
    if (first == last)
        return;
    for (SKeyPair8* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}