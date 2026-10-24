// std::__uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&) -- copy-construct n copies of x into raw storage at first and return first + n; T = a 0x18-byte {4 longs; two BGRA colours} element
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00720310, length 144 bytes (`ret 8`).
// Identity: the catalog label "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is propagated across byte-identical bodies; this
//   is STLport's `__uninitialized_fill_n(first, n, x, __false_type)` (the fill path of vector::_M_fill_insert / resize /
//   _M_insert_overflow), returning first + n (`ret 8`: x and the empty tag on the stack, first/n in ecx/edx).
// Shape: the element has a memberwise USER copy constructor and holds a member record that has its own user copy
//   constructor: VC7.1 then keeps a second induction register (`lea esi,[eax+N]`) pointing INTO the record and stores
//   the record's words relative to it; the placement new keeps the `test eax,eax` null check.
// Shape: each colour is copied through an inline Set(r, g, b, a) member (R, G, B, A stores in that order), which gives retail
//   the byte shuffle through bl/cl and the two [esp] byte spills; the IV points at the first colour +1 (`lea esi,[eax+11h]`).
// Caller: the vector insert-range path at 0x00721060.
#include <new>
struct __false_type { __false_type() {} };
struct SColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    SColour(const SColour& o) { Set(o.R, o.G, o.B, o.A); }
};
struct SRec { long a, b, c; SRec(const SRec& o) : a(o.a), b(o.b), c(o.c) {} };
struct SElem { long m0, m4, m8, mc; SColour c0, c1; SElem(const SElem& o) : m0(o.m0), m4(o.m4), m8(o.m8), mc(o.mc), c0(o.c0), c1(o.c1) {} };
namespace std {
SElem* __fastcall __uninitialized_fill_n(SElem* first, unsigned long n, const SElem& x, const __false_type&)
{
    SElem* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SElem(x);
    return cur;
}
}