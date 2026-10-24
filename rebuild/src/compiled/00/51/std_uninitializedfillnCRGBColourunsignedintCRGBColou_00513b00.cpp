// std::__uninitialized_fill_n<CRGBColour*,unsigned int,CRGBColour>(CRGBColour* first, unsigned int n, const CRGBColour& x, const __false_type&) -- STLport uninitialised fill: placement copy-construct n colours from x, return the advanced pointer
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513B00, 70 bytes, `ret 8` (__fastcall: first in ecx, n in edx, the value and
//   the tag references on the stack -- a /Gr algorithm TU).
// Callers: 0x00515CAF, 0x005179FF, 0x006245EF (vector<CRGBColour>(n, x) / resize paths).
// Label correction: the catalog's `_Fill_n<unsigned long*,unsigned int,unsigned long>` is a propagated Dinkumware name;
//   the body is STLport's `__uninitialized_fill_n` with a placement-new null test per element.
// Element: a 4-byte element copied as four single bytes in the permuted order +2, +1, +0, +3 is CRGBColour's copy
//   constructor through the inline Set(r, g, b, a) over the D3DCOLOR byte order (B, G, R, A) -- the same copy as the
//   landed __uninitialized_copy<CRGBColour*> 0x0082C690. Set's arguments evaluate right to left, so B and G spill into
//   the `push ecx` frame word and the dead x argument slot before the R, G, B, A stores.
// Shape: `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- unsigned count (`test edx,edx ; jbe`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct CRGBColour {                             // 4 bytes: the union's channel view in D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

namespace std {

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template CRGBColour* __fastcall __uninitialized_fill_n<CRGBColour*, unsigned int, CRGBColour>(CRGBColour* first, unsigned int n,
    const CRGBColour& x, const __false_type&);

}