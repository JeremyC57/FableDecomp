// std::_Vector_base<SRecord20, std::allocator<SRecord20> >::_Vector_base(size_t n, const allocator<SRecord20>& a) -- STLport vector base sized constructor for 20-byte records: null the three pointers, allocate n records, finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe @ 0x005BA5AD, 51 bytes, `ret 8` (thiscall: n and the allocator reference on the stack; returns this).
// Only caller: the vector copy constructor @ 0x005BBDF1 (`_Vector_base(x.size(), x.get_allocator())`: the allocator
// temporary lives in the dead high byte of its argument slot, `lea eax,[ebp+0Bh]`; the size is `(last-first)/0x14`).
// Callees: std::allocator<SRecord20>::allocate(n, hint = 0) @ 0x005B9DE7 (landed as CRetailAllocator_20::allocate: `count ?
// malloc(count * 20) : 0`, `ret 8`), called on the end-of-storage proxy (`lea edi,[esi+8] ; mov ecx,edi`).
// Body: STLport 4.x _vector.h
//     _Vector_base(size_t __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
//     { _M_start = _M_end_of_storage.allocate(__n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + __n; }
// with _M_end_of_storage an _STLP_alloc_proxy<T*, T, allocator<T> > (empty allocator base + the pointer, at +8).
// Shape: size TU (the allocate call is out of line, `lea ecx,[ebx+ebx*4] ; lea eax,[eax+ecx*4]` = start + n*20). The element
// type is not recoverable from the body (a 20-byte record of the clothing inventory TU); named by its size.
#pragma optimize("s", on)
#include <stddef.h>
struct SRecord20 {
    long Value[5];                                         // 0x14 bytes
};
namespace std {
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n, const void* hint = 0);           // 0x005B9DE7
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
template class std::_Vector_base<SRecord20, std::allocator<SRecord20> >;