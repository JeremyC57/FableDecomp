// _global::InsertionSort_0086c830(SPair* first, SPair* last, SCompare comp) -- insertion-sort [first, last) of 16-byte {key, 3-word POD} elements by linear insertion of each element
// Leaf: InsertionSort_0086c830
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086C830, length 83 bytes (`ret 4`).
// Identity: the catalog `_Move<...>` label is propagated; the landed callers (SGI `sort` / `__final_insertion_sort`) name this
//   body `__insertion_sort`. Callee: `__linear_insert(first, last, value, comp)` @ 0x0086C620 (`__fastcall`; its manifest
//   label is propagated too). The element type stays descriptive (key word first, as the CKeyPairCompareLess callers sort).
// Callers: the landed __final_insertion_sort 0x0086CFF0 and the landed sort 0x0086DD80 (two calls each).
// Shape: SGI/STLport `__insertion_sort(first, last, comp)`, `__fastcall` (first in ecx, last in edx, the 4-byte functor on
//   the stack, `ret 4`): an empty range returns at once; for every i in [first + 1, last) the out-of-line
//   `__linear_insert(first, i, *i, comp)` is called with the element passed BY VALUE (STLport supplies `*__i` as `__val`):
//   the functor is pushed, `sub esp,N ; mov eax,esp` reserves the value slot and the copy constructor fills it in place.
//   STLport's pair has a USER copy constructor (`first(p.first), second(p.second)`): the key word is stored first, then
//   the POD second is block-copied through `add eax,4 ; mov edx,edi` with an induction register at &i->second. With the
//   implicit pair copy constructor the whole 16 bytes go as one block (tried).
struct SRecord12 {                              // 3-word POD (implicit copy: block copy through pointer registers)
    long m_a, m_b, m_c;
};

struct SPair {                                  // 16-byte {key, POD record} element
    long        first;                          // +0x00
    SRecord12   second;                         // +0x04
    SPair(const SPair& p) : first(p.first), second(p.second) { }   // STLport pair's user copy constructor
};

struct SCompare {                               // 4-byte comparison functor passed by value (plain dword push)
    void* m_context;
};

void __fastcall LinearInsert_0086c620(SPair* first, SPair* last, SPair value, SCompare comp);   // 0x0086C620

void __fastcall InsertionSort_0086c830(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert_0086c620(first, i, *i, comp);
}