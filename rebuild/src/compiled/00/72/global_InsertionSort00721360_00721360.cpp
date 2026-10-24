// _global::InsertionSort_00721360(SPair* first, SPair* last, SCompare comp) -- insertion-sort [first, last) of 0x18-byte {key, 3 words, 2 colours} elements by linear insertion of each element
// Leaf: InsertionSort_00721360
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721360, length 158 bytes (`ret 4`).
// Identity: the catalog `_Move<...>` label is propagated; the landed callers (SGI `sort` / `__final_insertion_sort`) name this
//   body `__insertion_sort`. Callee: `__linear_insert(first, last, value, comp)` @ 0x00720CB0 (`__fastcall`; its manifest
//   label is propagated too). The element type stays descriptive (key word first, as the CKeyPairCompareLess callers sort).
// Callers: the landed __final_insertion_sort 0x007218E0 (two calls).
// Shape: SGI/STLport `__insertion_sort(first, last, comp)`, `__fastcall` (first in ecx, last in edx, the 4-byte functor on
//   the stack, `ret 4`): an empty range returns at once; for every i in [first + 1, last) the out-of-line
//   `__linear_insert(first, i, *i, comp)` is called with the element passed BY VALUE (STLport supplies `*__i` as `__val`):
//   the functor is pushed, `sub esp,N ; mov eax,esp` reserves the value slot and the copy constructor fills it in place.
//   The two CRGBColour members copy through the landed `Set(other.R, other.G, other.B, other.A)` copy constructor: the
//   argument loads come before the stores, which needs three byte registers (bl/cl/dl) plus a spill byte in the frame
//   (`sub esp,8`), so `first` is spilled too and re-read for the call. Byte-wise member copies give load/store pairs
//   with no spill (tried).
struct CRGBColour {                             // B, G, R, A bytes; copy through Set (all four loads before the stores)
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct SPair {                                  // 0x18-byte element
    long        first;                          // +0x00
    long        m_4, m_8, m_c;                  // +0x04 .. +0x0c
    CRGBColour  colour0;                        // +0x10
    CRGBColour  colour1;                        // +0x14
};

struct SCompare {                               // 4-byte comparison functor passed by value (plain dword push)
    void* m_context;
};

void __fastcall LinearInsert_00720cb0(SPair* first, SPair* last, SPair value, SCompare comp);   // 0x00720CB0

void __fastcall InsertionSort_00721360(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert_00720cb0(first, i, *i, comp);
}