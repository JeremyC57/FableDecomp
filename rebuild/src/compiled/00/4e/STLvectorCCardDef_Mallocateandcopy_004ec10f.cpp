// _STL::vector<CCardDef>::_M_allocate_and_copy(size_t n, const CCardDef* first, const CCardDef* last) -- STLport vector regrowth helper: malloc n 68-byte card defs (none for zero) and copy-construct [first, last) into them
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EC10F, 68 bytes, `ret 0Ch` (thiscall; `this` is never read: the allocator is empty).
// Caller: vector<CCardDef>::operator= @ 0x004EBF32 (this chunk, the reallocation branch).
// Label correction: the catalog's "CShoveHaPennyRegionDef_AllocateCopyConstruct_0x44" is wrong; the element is CCardDef
//   (retail 0x44, vtable 0x01243A44 stored by the _Construct callee).
// Callees: CRT malloc @ 0x00BFEA0E, _Construct(CCardDef*, const CCardDef&) @ 0x004EC08A (/Gr __fastcall: slot in ecx,
//   source in edx).
// Shape: STLport `pointer _M_allocate_and_copy(n, first, last) { pointer result = _M_end_of_storage.allocate(n);
//   __uninitialized_copy(first, last, result, _IsPODType()); return result; }`: `allocate(n)` = `n != 0 ? malloc(n * 0x44)
//   : 0` (`imul eax,eax,44h`) and the `__false_type` copy loop inlined (destination walked as `src + (result - first)`),
//   _Construct out of line. Under "s" allocate and __uninitialized_copy must be `__forceinline` (the landed-style recipe
//   of 0x004E6088). Size TU without a frame pointer: `#pragma optimize("s", on)` (`pop ecx` after malloc).
// Model: no engine header for CCardDef (Ego_r.pdb gives 72 bytes, retail 0x44); local polymorphic model.
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct CParentDefClassBase {                    // retail 0x3C bytes
    virtual void v00();
    unsigned char _body[0x38];                  // +0x04 def-class base body
};
class CCardDef : public CParentDefClassBase {   // 0x44 bytes, vtable 0x01243A44
public:
    long CardName;                              // +0x3C
    long CardVal;                               // +0x40
};

namespace _STL {
struct __false_type { __false_type() { } };
template <class T> void __fastcall _Construct(T* p, const T& value);   // 0x004EC08A
template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class T> struct allocator {           // STLport allocator<T>: empty
    allocator() { }
    __forceinline T* allocate(size_t n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> struct vector {              // STLport vector (12 bytes in retail)
    T*                   _M_start;              // +0
    T*                   _M_finish;             // +4
    _STLP_alloc_proxy<T> _M_end_of_storage;     // +8
    T* _M_allocate_and_copy(size_t n, const T* first, const T* last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
};
template CCardDef* vector<CCardDef>::_M_allocate_and_copy(size_t, const CCardDef*, const CCardDef*);
}