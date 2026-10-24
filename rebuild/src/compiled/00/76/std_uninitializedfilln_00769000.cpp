// std::__uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&) -- copy-construct n copies of x into raw storage at first and return first + n; T = a 16-byte {long; 3-long record} element
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769000, length 55 bytes (`ret 8`).
// Identity: the catalog label "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is propagated across byte-identical bodies; this
//   is STLport's `__uninitialized_fill_n(first, n, x, __false_type)` (the fill path of vector::_M_fill_insert / resize /
//   _M_insert_overflow), returning first + n (`ret 8`: x and the empty tag on the stack, first/n in ecx/edx).
// Shape: the element has a memberwise USER copy constructor and holds a member record that has its own user copy
//   constructor: VC7.1 then keeps a second induction register (`lea esi,[eax+N]`) pointing INTO the record and stores
//   the record's words relative to it; the placement new keeps the `test eax,eax` null check.
// Caller: the vector fill-insert at 0x0076ADA0.
#include <new>
struct __false_type { __false_type() {} };
struct S12 { long a, b, c; S12(const S12& o) : a(o.a), b(o.b), c(o.c) {} };
struct SElement16 {
    long first; S12 second;
    SElement16(const SElement16& o) : first(o.first), second(o.second) {}
};
namespace std {
SElement16* __fastcall __uninitialized_fill_n(SElement16* first, unsigned long n, const SElement16& x, const __false_type&)
{
    SElement16* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SElement16(x);
    return cur;
}
}