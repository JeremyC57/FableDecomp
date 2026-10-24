// std::__uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&) -- copy-construct n copies of x into raw storage at first and return first + n; T = a 16-byte {long; polymorphic {vptr, long, long}} element
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0F6B0, length 56 bytes (`ret 8`).
// Identity: the catalog label "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is propagated across byte-identical bodies; this
//   is STLport's `__uninitialized_fill_n(first, n, x, __false_type)` (the fill path of vector::_M_fill_insert / resize /
//   _M_insert_overflow), returning first + n (`ret 8`: x and the empty tag on the stack, first/n in ecx/edx).
// Shape: the element has a memberwise USER copy constructor and holds a member record that has its own user copy
//   constructor: VC7.1 then keeps a second induction register (`lea esi,[eax+N]`) pointing INTO the record and stores
//   the record's words relative to it; the placement new keeps the `test eax,eax` null check.
// Shape: the polymorphic member's user copy constructor stores its vtable 0x0125FEE4 (`mov [esi-8],imm`) between the words.
// Caller: the vector reallocation at 0x00B0F9F0.
#include <new>
struct __false_type { __false_type() {} };
struct CPoly { virtual void v0(); long a, b; CPoly(const CPoly& o) : a(o.a), b(o.b) {} };
struct SElem { long m0; CPoly poly; SElem(const SElem& o) : m0(o.m0), poly(o.poly) {} };
namespace std {
SElem* __fastcall __uninitialized_fill_n(SElem* first, unsigned long n, const SElem& x, const __false_type&)
{
    SElem* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SElem(x);
    return cur;
}
}