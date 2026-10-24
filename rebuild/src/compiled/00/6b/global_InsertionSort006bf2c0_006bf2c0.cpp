// _global::InsertionSort_006bf2c0(SPair* first, SPair* last, SCompare comp) -- insertion-sort [first, last) of 0x18-byte {key, {2 wide strings, 2 words, flag}} elements by linear insertion of each element
// Leaf: InsertionSort_006bf2c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BF2C0, length 118 bytes (`ret 4`).
// Identity: the catalog `_Move<...>` label is propagated; the landed callers (SGI `sort` / `__final_insertion_sort`) name this
//   body `__insertion_sort`. Callee: `__linear_insert(first, last, value, comp)` @ 0x006BF020 (`__fastcall`; its manifest
//   label is propagated too). The element type stays descriptive (key word first, as the CKeyPairCompareLess callers sort).
// Callers: the landed __final_insertion_sort 0x006BF480 (two calls).
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720 (twice per element, into the argument slot).
// Shape: SGI/STLport `__insertion_sort(first, last, comp)`, `__fastcall` (first in ecx, last in edx, the 4-byte functor on
//   the stack, `ret 4`): an empty range returns at once; for every i in [first + 1, last) the out-of-line
//   `__linear_insert(first, i, *i, comp)` is called with the element passed BY VALUE (STLport supplies `*__i` as `__val`):
//   the functor is pushed, `sub esp,N ; mov eax,esp` reserves the value slot and the copy constructor fills it in place.
//   The implicit copy constructor builds the value in the argument slot: key word, then the nested value with
//   `lea edi,[eax+4]` as its this (two wide-string copies, two words, a flag byte); `first` is kept in a frame slot.
struct CWideString {                            // 4-byte counted wide-string handle
    void* m_pData;
    CWideString(const CWideString& other);      // 0x0099B720
};

struct SValue {                                 // 0x14-byte value
    CWideString m_a;                            // +0x00
    CWideString m_b;                            // +0x04
    long        m_c;                            // +0x08
    long        m_d;                            // +0x0c
    bool        m_e;                            // +0x10
};

struct SPair {                                  // 0x18-byte {key, value} element
    long        first;                          // +0x00
    SValue      second;                         // +0x04
};

struct SCompare {                               // 4-byte comparison functor passed by value (plain dword push)
    void* m_context;
};

void __fastcall LinearInsert_006bf020(SPair* first, SPair* last, SPair value, SCompare comp);   // 0x006BF020

void __fastcall InsertionSort_006bf2c0(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert_006bf020(first, i, *i, comp);
}