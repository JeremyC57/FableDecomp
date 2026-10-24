// std::vector<CCharString>::_M_allocate_and_copy(unsigned int, const CCharString*, const CCharString*) -- allocate n strings and copy-construct [first, last) into them
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00432FF0, length 80 bytes, `ret 0xc` (__fastcall; `this` unused), returns the block.
// Name: STLport 4.x `vector::_M_allocate_and_copy(n, first, last)` for 4-byte CCharString elements (`shl eax,2`, the copy
// constructor per element); single caller 0x00432F25 (the vector assignment). The catalog's "Vector_AllocateAndCopyCCharString" is descriptive.
// Callees: malloc @ 0x00BFEA0E (`__cdecl`, `pop ecx`), CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Pragma: #pragma optimize("s", on) -- `pop ecx` after the cdecl call (size TU).
// Shape: the twin of 0x0043212B (pair<unsigned long, CSubDefInfo> elements): allocator<T>::allocate is `n != 0 ? malloc(n *
// sizeof(T)) : 0` (the block kept in ebx), then the non-POD __uninitialized_copy loop `for (; first != last; ++first, ++cur)
// _Construct(&*cur, *first)` -- the placement new's null test on `first + (result - first)` (edi = result - first), then the
// out-of-line copy constructor. allocate, _Construct and __uninitialized_copy are __forceinline (size TU).
#pragma optimize("s", on)
#include <stdlib.h>
#include <new>
class CCharStringData;
class CCharString {
public:
    CCharString(const CCharString& other);               // 0x0099EC30
    CCharStringData* PStringData;
};
namespace std {
template <class T> class allocator {
public:
    __forceinline T* allocate(size_t n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class T, class A = allocator<T> > class vector {
public:
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef size_t   size_type;
    T* _M_start;
    T* _M_finish;
    A  _M_end_of_storage_alloc;
    T* _M_end_of_storage;
    pointer _M_allocate_and_copy(size_type n, const_pointer first, const_pointer last)
    {
        pointer result = _M_end_of_storage_alloc.allocate(n);
        __uninitialized_copy(first, last, result);
        return result;
    }
};
template class vector<CCharString>;
}