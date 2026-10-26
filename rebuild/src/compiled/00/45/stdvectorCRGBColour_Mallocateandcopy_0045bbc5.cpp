// std::vector<CRGBColour>::_M_allocate_and_copy(unsigned int n, const CRGBColour* first, const CRGBColour* last) -- STLport: malloc n colours (none for zero), copy-construct [first, last) into them through _Construct, return the new block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045BBC5, length 68 bytes, `ret 0Ch` (thiscall: n, first, last).
// Callees: malloc @ 0x00BFEA0E, std::_Construct<CRGBColour, CRGBColour> @ 0x0045BB8F (`__fastcall`, this chunk).
// Label: the catalog "Vector_AllocateAndCopyRGB" is behavioural; the single caller is vector<CRGBColour>::operator= @ 0x0045BAAA.
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) (CRGBColour
// has a user copy ctor, so it is not a POD for the type traits), `_Construct(&*cur, *first)` per element, out of line.
// Shape: `pop ecx` after malloc, no frame pointer = size TU (#pragma optimize("s", on)); free templates `__fastcall` (/Gr).
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

class CRGBColour { public:                                // D3DCOLOR byte order, 4 bytes
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& other);
};

namespace std {
struct __false_type { };

template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value);   // 0x0045BB8F

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);
};

template <class T, class A> T* vector<T, A>::_M_allocate_and_copy(unsigned int n, const T* first, const T* last)
{
    T* result = _M_end_of_storage.allocate(n);
    __uninitialized_copy(first, last, result, __false_type());
    return result;
}

template class vector<CRGBColour, allocator<CRGBColour> >;
}