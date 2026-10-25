// std::vector<CEngineGraphic>::_M_allocate_and_copy(size_t n, const CEngineGraphic* first, const CEngineGraphic* last) -- STLport vector regrowth helper for CReplaceableMeshes' CArray<CEngineGraphic> (14-byte packed records): malloc n elements (none for zero) and copy-construct [first, last) into them
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6088, 66 bytes, `ret 0Ch` (size-optimised TU: unaligned entry,
//   `pop ecx` after malloc). `this` (the vector) is never read: the allocator is empty.
// Caller: the vector's operator= at 0x004E5F70 (catalogued as the propagated `CActiveFile::operator_assign`), next to
//   CReplaceableMeshDef::Copy 0x004E5F51.
// Callee: CRT malloc 0x00BFEA0E.
// Label correction: the catalog's `AllocateAndCopyActiveFileStride14` is descriptive; the element is CEngineGraphic
//   (Ego_r.pdb, 14 bytes: BankIndex +0, AnimStep +4, RenderSizeX +8, AdditiveAlpha +0xc, Type +0xd), the element of
//   CReplaceableMeshes::Vector (CArray<CEngineGraphic>).
// Shape: STLport `pointer _M_allocate_and_copy(size_type n, const_iterator first, const_iterator last)
//   { pointer result = _M_end_of_storage.allocate(n); __uninitialized_copy(first, last, result, _IsPODType());
//   return result; }` with `allocate(n)` = `n != 0 ? malloc(n * 14) : 0` and the `__false_type` copy loop inlined
//   (allocate, _Construct and the copy loop `__forceinline` under `optimize("s")`): the placement-new null test per element, the implicit 14-byte copy as
//   `movsd x3 ; movsw` (the record is 2-byte packed: `#pragma pack(2)`, the PDB size).
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

namespace std {

struct __false_type { __false_type() { } };

#pragma pack(push, 2)
struct CEngineGraphic {                         // Ego_r.pdb, 14 bytes
    unsigned long BankIndex;                    // +0x00
    float         AnimStep;                     // +0x04
    float         RenderSizeX;                  // +0x08
    bool          AdditiveAlpha;                // +0x0c
    unsigned char Type;                         // +0x0d
};
#pragma pack(pop)

template <class T> __forceinline void _Construct(T* p, const T& value) { new (p) T(value); }

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

template CEngineGraphic* vector<CEngineGraphic>::_M_allocate_and_copy(size_t, const CEngineGraphic*, const CEngineGraphic*);

}