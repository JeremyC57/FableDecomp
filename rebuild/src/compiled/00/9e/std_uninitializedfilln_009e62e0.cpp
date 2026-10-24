// std::__uninitialized_fill_n(T* first, unsigned long n, const T& x, const __false_type&) -- copy-construct n copies of x into raw storage at first and return first + n; T = a 16-byte {CCharString; {CCharString, CCharString, long}} element
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E62E0, length 90 bytes (`ret 8`).
// Identity: the catalog label "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is propagated across byte-identical bodies; this
//   is STLport's `__uninitialized_fill_n(first, n, x, __false_type)` (the fill path of vector::_M_fill_insert / resize /
//   _M_insert_overflow), returning first + n (`ret 8`: x and the empty tag on the stack, first/n in ecx/edx).
// Shape: the element has a memberwise USER copy constructor and holds a member record that has its own user copy
//   constructor: VC7.1 then keeps a second induction register (`lea esi,[eax+N]`) pointing INTO the record and stores
//   the record's words relative to it; the placement new keeps the `test eax,eax` null check.
// Callees: CCharString copy constructor 0x0099EC30 (three per element; the count is spilled to [esp]).
// Caller: the vector push_back at 0x009E77E0.
#include <new>
struct __false_type { __false_type() {} };
struct CCharStringData;
struct CCharString { CCharStringData* PStringData; CCharString(const CCharString& other); };   // 0x0099EC30
struct SRec { CCharString a, b; long c; SRec(const SRec& o) : a(o.a), b(o.b), c(o.c) {} };
struct SElem { CCharString name; SRec rec; SElem(const SElem& o) : name(o.name), rec(o.rec) {} };
namespace std {
SElem* __fastcall __uninitialized_fill_n(SElem* first, unsigned long n, const SElem& x, const __false_type&)
{
    SElem* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SElem(x);
    return cur;
}
}