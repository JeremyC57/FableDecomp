// std::_Vector_base<bool>::_Vector_base(unsigned int n, const allocator<bool>& a) -- STLport vector<bool> storage (1-byte elements): null the three pointers, malloc n bytes (none for zero), finish = start, end-of-storage = start + n
// Leaf: ??0?$_Vector_base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00450CCC, length 46 bytes, `ret 8` (thiscall: n, const allocator&).
// Callees: malloc @ 0x00BFEA0E (`__cdecl`, the STLport allocator<T>::allocate body).
// Label: the catalog "Buffer_InitializeWithSize" is behavioural. Callers: 0x00451583 (vector<bool>(n, value, a),
// in this chunk) and 0x00D34F15. Element size 1 (the end pointer is start + n unscaled): the IsUsable vector<bool> of
// CPlayerInventoryDef (retail STLport keeps vector<bool> a plain byte vector; see the landed 0x00462E2D).
// Model: the landed STLport _Vector_base / _STLP_alloc_proxy model of 0x007C2AB0 (CSTLportVector<C3DVector>), here emitted
// out of line; allocate() and the proxy ctor are __forceinline (a size TU leaves them out of line otherwise).
// Shape: size TU (`pop ecx` after the malloc, the zero register stored before the null test) -- #pragma optimize("s", on).
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

namespace std {
template <class T> struct allocator {           // STLport allocator<T>: empty
    __forceinline allocator() { }
    __forceinline T* allocate(unsigned int count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};

template <class P, class T, class A> struct _STLP_alloc_proxy : A {   // the end-of-storage pointer over the empty allocator
    P _M_data;
    __forceinline _STLP_alloc_proxy(const A& a, P p) : A(a), _M_data(p) { }
};

template <class T, class A> struct _Vector_base {
    T*                              _M_start;            // +0
    T*                              _M_finish;           // +4
    _STLP_alloc_proxy<T*, T, A>     _M_end_of_storage;   // +8
    _Vector_base(unsigned int n, const A& a);
};

template <class T, class A> _Vector_base<T, A>::_Vector_base(unsigned int n, const A& a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
{
    _M_start = _M_end_of_storage.allocate(n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + n;
}

template struct _Vector_base<bool, allocator<bool> >;
}