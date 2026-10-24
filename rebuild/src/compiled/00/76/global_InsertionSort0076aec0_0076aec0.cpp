// _global::InsertionSort_0076aec0(SPair* first, SPair* last, SCompare comp) -- insertion-sort [first, last) of 16-byte {key, 3-long record} elements by linear insertion of each element
// Leaf: InsertionSort_0076aec0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076AEC0, length 79 bytes (`ret 4`).
// Identity: the catalog `_Move<...>` label is propagated; the landed callers (SGI `sort` / `__final_insertion_sort`) name this
//   body `__insertion_sort`. Callee: `__linear_insert(first, last, value, comp)` @ 0x0076AAE0 (`__fastcall`; its manifest
//   label is propagated too). The element type stays descriptive (key word first, as the CKeyPairCompareLess callers sort).
// Callers: the landed SGI sort 0x0076C000 (inlined __final_insertion_sort, two calls) and the assign helper 0x0076B320.
// Shape: SGI/STLport `__insertion_sort(first, last, comp)`, `__fastcall` (first in ecx, last in edx, the 4-byte functor on
//   the stack, `ret 4`): an empty range returns at once; for every i in [first + 1, last) the out-of-line
//   `__linear_insert(first, i, *i, comp)` is called with the element passed BY VALUE (STLport supplies `*__i` as `__val`):
//   the functor is pushed, `sub esp,N ; mov eax,esp` reserves the value slot and the copy constructor fills it in place.
//   The record's user copy constructor puts the induction register at +0x0c (= &second + 8); the functor is loaded before
//   the key word.
struct SRecord12 {                              // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SPair {                                  // 16-byte {key, record} element
    long        first;                          // +0x00
    SRecord12   second;                         // +0x04
};

struct SCompare {                               // 4-byte comparison functor passed by value (plain dword push)
    void* m_context;
};

void __fastcall LinearInsert_0076aae0(SPair* first, SPair* last, SPair value, SCompare comp);   // 0x0076AAE0

void __fastcall InsertionSort_0076aec0(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert_0076aae0(first, i, *i, comp);
}