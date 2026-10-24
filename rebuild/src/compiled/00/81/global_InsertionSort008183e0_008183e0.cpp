// _global::InsertionSort_008183e0(SPair* first, SPair* last, SCompare comp) -- insertion-sort [first, last) of 0x20-byte {key, word, 3-long record, 3 words} elements by linear insertion of each element
// Leaf: InsertionSort_008183e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008183E0, length 103 bytes (`ret 4`).
// Identity: the catalog `_Move<...>` label is propagated; the landed callers (SGI `sort` / `__final_insertion_sort`) name this
//   body `__insertion_sort`. Callee: `__linear_insert(first, last, value, comp)` @ 0x00818240 (`__fastcall`; its manifest
//   label is propagated too). The element type stays descriptive (key word first, as the CKeyPairCompareLess callers sort).
// Callers: the partial-sort helper 0x00818450 and the landed sort 0x00818C20 (two calls each).
// Shape: SGI/STLport `__insertion_sort(first, last, comp)`, `__fastcall` (first in ecx, last in edx, the 4-byte functor on
//   the stack, `ret 4`): an empty range returns at once; for every i in [first + 1, last) the out-of-line
//   `__linear_insert(first, i, *i, comp)` is called with the element passed BY VALUE (STLport supplies `*__i` as `__val`):
//   the functor is pushed, `sub esp,N ; mov eax,esp` reserves the value slot and the copy constructor fills it in place.
//   The induction register sits at +0x10 = &record + 8, so the user-copy record starts at +8 (a 4-long record at +4
//   puts it at +0x0c instead: tried).
struct SRecord12 {                              // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SPair {                                  // 0x20-byte element
    long        first;                          // +0x00
    long        second;                         // +0x04
    SRecord12   record;                         // +0x08
    long        m_14, m_18, m_1c;               // +0x14 .. +0x1c
};

struct SCompare {                               // 4-byte comparison functor passed by value (plain dword push)
    void* m_context;
};

void __fastcall LinearInsert_00818240(SPair* first, SPair* last, SPair value, SCompare comp);   // 0x00818240

void __fastcall InsertionSort_008183e0(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert_00818240(first, i, *i, comp);
}