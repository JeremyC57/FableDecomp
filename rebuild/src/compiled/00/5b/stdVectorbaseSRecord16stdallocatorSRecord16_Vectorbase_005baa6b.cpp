// std::_Vector_base<SRecord16, std::allocator<SRecord16> >::_Vector_base(size_t n, const allocator<SRecord16>& a) -- STLport vector base sized constructor for 16-byte records: null the three pointers, malloc n records (none for zero), finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe @ 0x005BAA6B, 54 bytes, `ret 8` (thiscall: n and the allocator reference on the stack; returns this).
// Only caller: the vector copy constructor @ 0x005BB346 (`_Vector_base(x.size(), x.get_allocator())`: `sar eax,4` size, the
// allocator temporary in the dead high byte of its argument slot, then the range copy @ 0x005BAF28).
// Callees: malloc @ 0x00BFEA0E (`__cdecl`, `pop ecx`).
// Body: STLport 4.x _vector.h
//     _Vector_base(size_t __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
//     { _M_start = _M_end_of_storage.allocate(__n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + __n; }
// Shape: unlike the 20-byte twins 0x005BA5AD / 0x005C0932 this instantiation's allocator::allocate (`n ? malloc(n*16) : 0`)
// is INLINED: the zero register doubles as the null result (`cmp edi,eax ; je` skips the call with eax still 0) and VC7.1
// sinks the end-of-storage store (`shl edi,4 ; add edi,eax`) ahead of the start/finish stores. Size TU. The element type is
// not recoverable from the body (a 16-byte record); named by its size.
#pragma optimize("s", on)
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);              // 0x00BFEA0E
struct SRecord16 {
    long Value[4];                                         // 0x10 bytes
};
namespace std {
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
}
template class std::_Vector_base<SRecord16, std::allocator<SRecord16> >;