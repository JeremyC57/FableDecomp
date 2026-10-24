// std::__uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&) -- copy-construct n copies of x into raw storage at first and return first + n; T = a 13-byte packed {unsigned char; 3-long record} element
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513A00, length 56 bytes (`ret 8`).
// Identity: the catalog label "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is propagated across byte-identical bodies; this
//   is STLport's `__uninitialized_fill_n(first, n, x, __false_type)` (the fill path of vector::_M_fill_insert / resize /
//   _M_insert_overflow), returning first + n (`ret 8`: x and the empty tag on the stack, first/n in ecx/edx).
// Shape: the element has a memberwise USER copy constructor and holds a member record that has its own user copy
//   constructor: VC7.1 then keeps a second induction register (`lea esi,[eax+N]`) pointing INTO the record and stores
//   the record's words relative to it; the placement new keeps the `test eax,eax` null check.
// Layout: the element stride is 0x0d and the record sits at +1 (`lea esi,[eax+9]` = record +8), so the retail element
//   really is byte-packed: pack(1) here states that layout, it does not hide one.
// Caller: the vector fill paths at 0x00514C10 / 0x00515720 (labels propagated).
#include <new>
struct __false_type { __false_type() {} };
#pragma pack(push, 1)
struct SRec { long a, b, c; SRec(const SRec& o) : a(o.a), b(o.b), c(o.c) {} };
struct SElem { unsigned char first; SRec second; SElem(const SElem& o) : first(o.first), second(o.second) {} };
#pragma pack(pop)
namespace std {
SElem* __fastcall __uninitialized_fill_n(SElem* first, unsigned long n, const SElem& x, const __false_type&)
{
    SElem* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SElem(x);
    return cur;
}
}