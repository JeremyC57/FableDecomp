// _global::UninitCopy_CRGBColour_0082c690(const CRGBColour* first, const CRGBColour* last, CRGBColour* result, const __false_type&) -- STLport __uninitialized_copy<CRGBColour*>: placement copy-construct colours channel by channel, return the advanced result
// Leaf: UninitCopy_CRGBColour_0082c690
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082C690, length 74 bytes (+6 int3 pad), `ret 8`.
// Callers: 0x0082C8E7 (vector construct 0x0082C8A0), 0x0082CC0C (vector copy 0x0082CB70) and 0x0082CCD9 (0x0082CC40).
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. A 4-byte
//   element copied as four single bytes in the permuted order +2, +1, +0, +3 is CRGBColour's copy constructor through
//   the inline Set(r, g, b, a) over the D3DCOLOR byte order (B, G, R, A), exactly as the landed CRGBColour copies
//   (0x00B1FE80, 0x005BA48A; GOTCHAS 004c5890).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx -> esi, result + the tag reference on the stack). Set's arguments are evaluated right to left (A, B, G, R loads),
//   the byte temporaries B and G spill into the `push ecx` frame word and the dead result argument slot, then the
//   stores run R, G, B, A; placement-new null test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

class CRGBColour { public:                             // 4 bytes: the union's channel view in D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

CRGBColour* __fastcall UninitCopy_CRGBColour_0082c690(const CRGBColour* first, const CRGBColour* last, CRGBColour* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CRGBColour(*first);
    return result;
}