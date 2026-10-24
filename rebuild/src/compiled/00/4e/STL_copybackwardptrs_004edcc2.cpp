// _STL::__copy_backward_ptrs(const CPrizeScoreDef* first, const CPrizeScoreDef* last, CPrizeScoreDef* result, const __false_type&) -- STLport backward copy for the non-POD prize score def: assign the 48-byte elements of [first, last) downward ending at result, return the new start
// Leaf: __copy_backward_ptrs
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDCC2, 81 bytes, `ret 8` (/Gr __fastcall: first in ecx, last in edx; result and the
//   tag reference on the stack).
// Caller: 0x004EDBD1, the vector<CPrizeScoreDef> insert path (shift the tail up one slot before assigning the new element;
//   the same body copy-constructs through CPrizeScoreDef's copy ctor 0x004EDC9B and _Construct 0x004EAC30).
// Label correction: the catalog's "Vector_InsertCopyRange_Backward_0x30" is descriptive; the element is CPrizeScoreDef
//   (0x30: the 0x28 CDefClassBase, Score +0x28, Mult +0x2C; vtable 0x012437D4 stored by its landed copy ctor 0x004EDC9B).
// Callee: CDefClassBase::operator=(const CDefClassBase&) @ 0x00431F10.
// Shape: STLport `__copy_backward_ptrs(first, last, result, __false_type)` = `__copy_backward(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)` inlined: `for (n = last - first; n > 0; --n) *--result = *--last;` with
//   the count from the size-TU division (`push 30h ; cdq ; pop ecx ; idiv ecx`), the running result in its argument slot
//   (`sub [ebp+8],30h`, re-homed: `mov ebx,[ebp+8] ; mov [ebp+8],ebx`), the source walked as `result + (last - result)`.
//   CPrizeScoreDef's implicit operator= is spelled out and __forceinline (base operator= out of line, then Score, then Mult),
//   as is __copy_backward (the size TU would out-line both). Frame-pointer size TU: `optimize("y", off)` + `("s", on)`.
// Model: no engine header for CPrizeScoreDef; CDefClassBase as in the landed 0x004E9BDA (0x25 bytes, pack 1).
#pragma optimize("y", off)
#pragma optimize("s", on)

#pragma pack(push, 1)
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x21];                       // +0x04..+0x25
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)
class CPrizeScoreDef : public CDefClassBase {        // vtable 0x012437D4, 0x30 bytes
public:
    float Score;                                     // +0x28
    long  Mult;                                      // +0x2c
    virtual void v00();
    __forceinline CPrizeScoreDef& operator=(const CPrizeScoreDef& o)
    {
        CDefClassBase::operator=(o);
        Score = o.Score;
        Mult = o.Mult;
        return *this;
    }
};
namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter first, _RandomAccessIter last, _BidirectionalIter result,
                                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
template <class _Tp>
_Tp* __fastcall __copy_backward_ptrs(const _Tp* first, const _Tp* last, _Tp* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
template CPrizeScoreDef* __fastcall __copy_backward_ptrs<CPrizeScoreDef>(const CPrizeScoreDef* first, const CPrizeScoreDef* last,
                                                                          CPrizeScoreDef* result, const __false_type&);
}