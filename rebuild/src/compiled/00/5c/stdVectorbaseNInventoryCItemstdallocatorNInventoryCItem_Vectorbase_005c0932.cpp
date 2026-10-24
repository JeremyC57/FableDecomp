// std::_Vector_base<NInventory::CItem, std::allocator<NInventory::CItem> >::_Vector_base(size_t n, const allocator<NInventory::CItem>& a) -- STLport vector base sized constructor for inventory items: null the three pointers, allocate n records, finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe @ 0x005C0932, 51 bytes, `ret 8` (thiscall: n and the allocator reference on the stack; returns this).
// Only caller: vector<NInventory::CItem>'s copy constructor @ 0x005C140E (landed; `_Vector_base(x.size(), x.get_allocator())`,
// the allocator temporary in the dead high byte of its argument slot, then the CItem range copy @ 0x005C10CC whose element
// copy constructor is 0x005C08E6).
// Callees: std::allocator<NInventory::CItem>::allocate(n, hint = 0) @ 0x005C0436 (landed as CRetailAllocator_20::allocate:
// `count ? malloc(count * 20) : 0`, `ret 8`), called on the end-of-storage proxy (`lea edi,[esi+8] ; mov ecx,edi`).
// Body: STLport 4.x _vector.h
//     _Vector_base(size_t __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
//     { _M_start = _M_end_of_storage.allocate(__n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + __n; }
// with _M_end_of_storage an _STLP_alloc_proxy<T*, T, allocator<T> > (empty allocator base + the pointer, at +8).
// Shape: size TU (the allocate call is out of line, `lea ecx,[ebx+ebx*4] ; lea eax,[eax+ecx*4]` = start + n*20). Byte twin of
// 0x005BA5AD (another 20-byte element's instantiation). Element: engine/CItem.h (0x14 bytes).
#pragma optimize("s", on)
#include <stddef.h>
#include "engine/CItem.h"
namespace std {
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n, const void* hint = 0);           // 0x005C0436
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
template class std::_Vector_base<CItem, std::allocator<CItem> >;