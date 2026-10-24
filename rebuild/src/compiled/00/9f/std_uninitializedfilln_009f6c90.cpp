// std::__uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&) -- copy-construct n copies of x into raw storage at first and return first + n; T = a 0x30-byte {12-byte array with its own copy ctor; long; 16-byte POD; long; 3-long record} element
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6C90, length 106 bytes (`ret 8`).
// Identity: the catalog label "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is propagated across byte-identical bodies; this
//   is STLport's `__uninitialized_fill_n(first, n, x, __false_type)` (the fill path of vector::_M_fill_insert / resize /
//   _M_insert_overflow), returning first + n (`ret 8`: x and the empty tag on the stack, first/n in ecx/edx).
// Shape: the element has a memberwise USER copy constructor and holds a member record that has its own user copy
//   constructor: VC7.1 then keeps a second induction register (`lea esi,[eax+N]`) pointing INTO the record and stores
//   the record's words relative to it; the placement new keeps the `test eax,eax` null check.
// Callees: the array member's copy constructor 0x009F66A0. The 16-byte POD has NO constructor (implicit copy: the
//   `lea ecx,[edi+10h] ; lea edx,[esi-1Ch]` block copy).
// Caller: the vector insert-range at 0x009F6ED0.
#include <new>
struct __false_type { __false_type() {} };
struct CArray12 { void* m_first; void* m_last; void* m_end; CArray12(const CArray12& o); };   // 0x009F66A0
struct S16 { long a, b, c, d; };
struct SRec { long a, b, c; SRec(const SRec& o) : a(o.a), b(o.b), c(o.c) {} };
struct SElem { CArray12 arr; long mc; S16 s; long m20; SRec rec; SElem(const SElem& o) : arr(o.arr), mc(o.mc), s(o.s), m20(o.m20), rec(o.rec) {} };
namespace std {
SElem* __fastcall __uninitialized_fill_n(SElem* first, unsigned long n, const SElem& x, const __false_type&)
{
    SElem* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SElem(x);
    return cur;
}
}