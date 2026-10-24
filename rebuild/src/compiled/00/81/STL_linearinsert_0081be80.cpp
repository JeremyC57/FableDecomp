// _STL::__linear_insert(SKeyedPair12* first, SKeyedPair12* last, SKeyedPair12 val, CKeyPairCompareLess comp) -- insertion-sort step over (unsigned long key, 8-byte payload) pairs: a value smaller than the first element shifts [first, last) up one slot and goes to the front, else it is linearly inserted from the back
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081BE80, 91 bytes, `ret 10h` (link-time __fastcall /Gr TU: first in ecx, last in
//   edx, the 12-byte value and the empty comparator on the stack).
// Callees: __copy_backward(first, last, last + 1, random_access_iterator_tag(), (int*)0) @ 0x00819720 (landed, `ret 0Ch`),
//   __unguarded_linear_insert(last, val, comp) @ 0x00819A60 (`ret 10h`).
// Label: the catalog's "std_InsertionSort_InsertElement" is descriptive; the body is STLport `__linear_insert` (the
//   __insertion_sort step) over the 12-byte pairs with an UNSIGNED first-dword key -- the stride and key of the
//   12-byte __make_heap 0x0081BBD0 / __adjust_heap 0x0081AC20 family (CSkeletalMorphs-style pair<unsigned long, CEntry>).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- STLport's const-reference iterator tag is an empty temporary homed in
//   the comparator's slot (dead on that path, `lea eax,[esp+1Ch]`); `*first = val` is the trivial three-dword assignment;
//   the by-value val for the callee is rebuilt in the argument area by pair's memberwise copy ctor.
struct SPayload8 {                          // 8-byte POD payload
    long m_a;                               // +0x00
    long m_b;                               // +0x04
};

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class A, class B> struct pair {
    A first;                                // +0x00
    B second;                               // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<unsigned long, SPayload8> SKeyedPair12;

struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SKeyedPair12& a, const SKeyedPair12& b) const { return a.first < b.first; }
};

SKeyedPair12* __fastcall __copy_backward(SKeyedPair12* first, SKeyedPair12* last, SKeyedPair12* result,
                                         const random_access_iterator_tag&, int*);                            // 0x00819720
void __fastcall __unguarded_linear_insert(SKeyedPair12* last, SKeyedPair12 val, CKeyPairCompareLess comp);    // 0x00819A60

inline SKeyedPair12* copy_backward(SKeyedPair12* first, SKeyedPair12* last, SKeyedPair12* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(SKeyedPair12* first, SKeyedPair12* last, SKeyedPair12 val, CKeyPairCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
}