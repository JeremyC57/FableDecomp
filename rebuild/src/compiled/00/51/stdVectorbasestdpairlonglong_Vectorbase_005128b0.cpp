// std::_Vector_base<std::pair<long, long>>::_Vector_base(size_t n, const allocator_type& a) -- STLport vector base sized constructor: null the three pointers, malloc n elements (none for zero), finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@U?$pair@JJ@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005128B0, 56 bytes, thiscall, `ret 8` (n and the allocator reference; returns this).
// Identity (proven): its only caller is the vector copy constructor 0x005BAEC4, bound ??0?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAE@ABV01@@Z (8-byte stride).
// Callee: malloc 0x00BFEA0E (`n ? malloc(n * size) : 0`, the zero register doubling as the null result).
// Shape (family K126, representative 0x005128B0): the landed G010 _Vector_base 0x004F9450 body in a speed TU (`add esp,4`).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);              // 0x00BFEA0E
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n, const void* = 0) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
};
template <class T, class Alloc>
class _Vector_base {
public:
    _Vector_base(size_t n, const Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;     // +0x08
};
template class _Vector_base<std::pair<long, long>, allocator<std::pair<long, long> > >;
}