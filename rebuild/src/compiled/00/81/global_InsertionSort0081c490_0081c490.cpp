// _global::InsertionSort_0081c490(SPair* first, SPair* last, SCompare comp) -- insertion-sort [first, last) of 0x18-byte {key, 5-word POD} elements by linear insertion of each element
// Leaf: InsertionSort_0081c490
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081C490, length 95 bytes (`ret 4`).
// Identity: the catalog `_Move<...>` label is propagated; the landed callers (SGI `sort` / `__final_insertion_sort`) name this
//   body `__insertion_sort`. Callee: `__linear_insert(first, last, value, comp)` @ 0x0081BDA0 (`__fastcall`; its manifest
//   label is propagated too). The element type stays descriptive (key word first, as the CKeyPairCompareLess callers sort).
// Callers: the __final_insertion_sort helper 0x0081D340 (two calls).
// Shape: SGI/STLport `__insertion_sort(first, last, comp)`, `__fastcall` (first in ecx, last in edx, the 4-byte functor on
//   the stack, `ret 4`): an empty range returns at once; for every i in [first + 1, last) the out-of-line
//   `__linear_insert(first, i, *i, comp)` is called with the element passed BY VALUE (STLport supplies `*__i` as `__val`):
//   the functor is pushed, `sub esp,N ; mov eax,esp` reserves the value slot and the copy constructor fills it in place.
//   STLport pair user copy constructor: key first, then the 5-word POD second through `add eax,4 ; mov edx,edi` (the
//   implicit pair copy constructor block-copies all six words: tried).
struct SRecord20 {                              // 5-word POD (implicit copy: block copy through pointer registers)
    long m_a, m_b, m_c, m_d, m_e;
};

struct SPair {                                  // 0x18-byte {key, POD record} element
    long        first;                          // +0x00
    SRecord20   second;                         // +0x04
    SPair(const SPair& p) : first(p.first), second(p.second) { }   // STLport pair's user copy constructor
};

struct SCompare {                               // 4-byte comparison functor passed by value (plain dword push)
    void* m_context;
};

void __fastcall LinearInsert_0081bda0(SPair* first, SPair* last, SPair value, SCompare comp);   // 0x0081BDA0

void __fastcall InsertionSort_0081c490(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert_0081bda0(first, i, *i, comp);
}