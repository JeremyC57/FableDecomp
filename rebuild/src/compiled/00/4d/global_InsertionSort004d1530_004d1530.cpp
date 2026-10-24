// _global::InsertionSort_004d1530(SPair* first, SPair* last, SCompare comp) -- insertion-sort [first, last) of 0x1c-byte {key, 6-word POD} elements by linear insertion of each element
// Leaf: InsertionSort_004d1530
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D1530, length 101 bytes (`ret 4`).
// Identity: the catalog `_Move<...>` label is propagated; the landed callers (SGI `sort` / `__final_insertion_sort`) name this
//   body `__insertion_sort`. Callee: `__linear_insert(first, last, value, comp)` @ 0x004D1000 (`__fastcall`; its manifest
//   label is propagated too). The element type stays descriptive (key word first, as the CKeyPairCompareLess callers sort).
// Callers: the __final_insertion_sort helper 0x004D1AE0 (two calls).
// Shape: SGI/STLport `__insertion_sort(first, last, comp)`, `__fastcall` (first in ecx, last in edx, the 4-byte functor on
//   the stack, `ret 4`): an empty range returns at once; for every i in [first + 1, last) the out-of-line
//   `__linear_insert(first, i, *i, comp)` is called with the element passed BY VALUE (STLport supplies `*__i` as `__val`):
//   the functor is pushed, `sub esp,N ; mov eax,esp` reserves the value slot and the copy constructor fills it in place.
//   STLport pair user copy constructor: key first, then the 6-word POD second through `add eax,4 ; mov edx,edi`; here
//   the key word is loaded before the functor (`mov ecx,[esi] ; mov eax,[esp+14h]`) -- the same source as 0x0086C830.
struct SRecord24 {                              // 6-word POD (implicit copy: block copy through pointer registers)
    long m_a, m_b, m_c, m_d, m_e, m_f;
};

struct SPair {                                  // 0x1c-byte {key, POD record} element
    long        first;                          // +0x00
    SRecord24   second;                         // +0x04
    SPair(const SPair& p) : first(p.first), second(p.second) { }   // STLport pair's user copy constructor
};

struct SCompare {                               // 4-byte comparison functor passed by value (plain dword push)
    void* m_context;
};

void __fastcall LinearInsert_004d1000(SPair* first, SPair* last, SPair value, SCompare comp);   // 0x004D1000

void __fastcall InsertionSort_004d1530(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert_004d1000(first, i, *i, comp);
}