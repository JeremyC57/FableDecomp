// std::__uninitialized_copy(const SColouredRecord18* first, const SColouredRecord18* last, SColouredRecord18* result, const __false_type&) -- 24-byte {four words, two CRGBColours} records: words by register, colours through CRGBColour::Set, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUSColouredRecord18@FableCopy_00720270@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00720270, length 151 bytes, `ret 8`.
// Element: 24 bytes = four dwords (+0..+0xC) then two CRGBColours (+0x10, +0x14); the colours copy through the inline
// CRGBColour::Set(r, g, b, a) (all four bytes loaded before the stores, one spilled to the `push ecx` word), the
// same shape as CCompositeBlendDesc's copy 0x006CAC20. The record's class is not established (caller 0x00721060,
// a vector range insert; no 24-byte PDB record has colours at +0x10/+0x14): descriptive names.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00720270 {
struct CRGBColour {                                     // D3DCOLOR byte order (Ego_r.pdb CRGBColour union: B, G, R, A)
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
struct SColouredRecord18 {                              // 24 bytes
    unsigned long m_key;                                // +0x00
    long          m_word4;                              // +0x04
    long          m_word8;                              // +0x08
    long          m_wordC;                              // +0x0C
    CRGBColour    m_colour0;                            // +0x10
    CRGBColour    m_colour1;                            // +0x14
};
}
namespace std {
FableCopy_00720270::SColouredRecord18* __fastcall __uninitialized_copy(const FableCopy_00720270::SColouredRecord18* first, const FableCopy_00720270::SColouredRecord18* last,
    FableCopy_00720270::SColouredRecord18* result, const __false_type&)
{
    FableCopy_00720270::SColouredRecord18* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00720270::SColouredRecord18(*first);
    return cur;
}
}