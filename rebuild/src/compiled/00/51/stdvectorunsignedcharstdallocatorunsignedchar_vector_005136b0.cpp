// std::vector<unsigned char,std::allocator<unsigned char> >::vector(unsigned int n) -- STLport vector(n) of bytes: allocate n bytes and zero them (the one-byte uninitialized_fill_n is a memset)
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005136B0, 77 bytes, `ret 4` (thiscall: n).
// Callers: 0x009FB346, 0x009FB63C, 0x009FB95F (CSurface / data-stream buffers), 0x00A68D05, 0x00A69052, 0x00A693B2.
//   Catalog label "LoadDataStream_AllocateSurface" is descriptive.
// Callee: malloc @ 0x00BFEA0E (`__cdecl`, result kept: `add esp,4`).
// Shape: STLport `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }`; for a one-byte POD STLport fills with memset, which VC7.1 expands
//   inline (`rep stosd` + `rep stosb`). One `xor edx,edx` serves the three null initialisers and the null block.
#include <string.h>

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace std {

inline unsigned char* uninitialized_fill_n(unsigned char* first, unsigned int n, const unsigned char& x)
{
    memset(first, x, n);
    return first + n;
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};

template class vector<unsigned char, allocator<unsigned char> >;

}