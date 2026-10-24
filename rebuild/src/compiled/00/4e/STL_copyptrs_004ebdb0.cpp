// _STL::__copy_ptrs(const CCardPositionsDef* first, const CCardPositionsDef* last, CCardPositionsDef* result, const __false_type&) -- STLport __copy_ptrs for the non-POD card-positions def: counted forward assignment of 72-byte elements, returns the advanced result
// Leaf: __copy_ptrs
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBDB0, 65 bytes, `ret 8` (/Gr __fastcall: first in ecx, last in edx; result and the
//   tag reference on the stack).
// Callers: vector<CCardPositionsDef>::operator= @ 0x004EBCA9 (this chunk; both copy branches).
// Label correction: the catalog's "VectorReallocateCopyConstruct" is descriptive; the element is CCardPositionsDef (0x48:
//   the 0x3C CParentDefClassBase + vector<C2DVector> Offset, the landed _Construct @ 0x004EBDF1).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0 (landed), vector<C2DVector>::operator= @ 0x0046EABD (catalog
//   "CActiveFile_Copy", propagated; its callers are def assignments copying C2DVector vectors).
// Shape: `__copy_ptrs(first, last, result, __false_type)` = `__copy(first, last, result, random_access_iterator_tag(),
//   (ptrdiff_t*)0)` inlined: `for (n = last - first; n > 0; --n) *result++ = *first++;` with the count from the size-TU
//   division (`push 48h ; cdq ; pop edi ; idiv edi`) and the source walked as `result + (first - result)`. The element
//   assignment is CCardPositionsDef's implicit operator= (base operator=, then Offset's), spelled out and __forceinline
//   because the size TU calls the implicit one out of line; __copy is __forceinline for the same reason (the landed
//   0x004E9BDA recipe). Size TU without a frame pointer: `#pragma optimize("s", on)`.
// Model: no engine header for CCardPositionsDef (Ego_r.pdb gives 80 bytes, retail 0x48); local model as in 0x004EBDF1.
#pragma optimize("s", on)
namespace _STL {
struct __false_type { };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}
struct CParentDefClassBase {                               // retail 0x3C bytes
    virtual void v00();
    unsigned char _body[0x38];                              // +0x04 def-class base body
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
struct C2DVector { float X, Y; };
struct C2DVectorVector {                                    // STLport vector<C2DVector>, 12 bytes
    C2DVector* _M_start;
    C2DVector* _M_finish;
    C2DVector* _M_end_of_storage;
    C2DVectorVector& operator=(const C2DVectorVector& other);   // 0x0046EABD
};
class CCardPositionsDef : public CParentDefClassBase {      // 0x48 bytes
public:
    C2DVectorVector Offset;                                 // +0x3C
    __forceinline CCardPositionsDef& operator=(const CCardPositionsDef& o)
    {
        CParentDefClassBase::operator=(o);
        Offset = o.Offset;
        return *this;
    }
};
namespace _STL {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class _Tp>
_Tp* __fastcall __copy_ptrs(const _Tp* first, const _Tp* last, _Tp* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template CCardPositionsDef* __fastcall __copy_ptrs<CCardPositionsDef>(const CCardPositionsDef* first, const CCardPositionsDef* last,
                                                                      CCardPositionsDef* result, const __false_type&);
}