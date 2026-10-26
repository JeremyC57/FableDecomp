// std::vector<CHeroMarriageInfo>::reserve(unsigned int) -- grow the marriage vector's storage to n records when the capacity is smaller
// Leaf: reserve
// Retail: Fable.exe (TLC, VC7.1) @ 0x005806DF, 123 bytes, `ret 4` (thiscall: n).
// Callees: vector::_M_allocate_and_copy(n, first, last) @ 0x0057F8A7, the end-of-storage allocator's
// allocate(n, 0) @ 0x0057EFF0 (landed CRetailAllocator_24), free @ 0x00BFEA14 (the inline `_M_clear` deallocate).
// Only caller 0x0057EBBA in the CTCHeroStats TU (24-byte records; vector<CHeroMarriageInfo> helpers 0x00580500
// push_back / 0x0057F24E __uninitialized_copy / 0x0057A3A2 copy ctor live beside it).
// Label: the catalog "Vector_Reallocate" is descriptive: the body is STLport 4.x `vector::reserve(size_type)`:
// `if (capacity() < n) { old = size(); tmp = start ? _M_allocate_and_copy(n, start, finish) : allocate(n);
// if (start) _M_clear(); _M_set(tmp, tmp + old, tmp + n); }`. The record has a trivial destructor, so `_M_clear`
// is only the null-tested `free` of the old block. `&_M_end_of_storage` is formed once (`lea eax,[esi+8]`) and
// kept in the frame word: the capacity reads `_M_end_of_storage._M_data` through it and the allocator is called on
// it; the old size reuses the dead `n` slot.
// Model: CHeroMarriageInfo modelled locally (24 bytes, PDB members; its copy ctor is out of line).
// Shape: frame-pointer size TU (`leave`, `push 18h ; pop edi`): `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class CHeroMarriageInfo { public:                          // 0x18 bytes (Ego_r.pdb)
    unsigned __int64 WifeUID;                       // +0x00
    unsigned __int64 MaritalHomeUID;                // +0x08
    long             State;                         // +0x10 EMarriageState
    bool             GayMarriage;                   // +0x14
    CHeroMarriageInfo(const CHeroMarriageInfo& other);   // 0x0057A3A2
};

namespace std {

template <class _Tp> struct allocator {
    _Tp* allocate(size_t __n, const void* __hint = 0);                      // 0x0057EFF0
    __forceinline void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp*                                                _M_start;             // +0x00
    _Tp*                                                _M_finish;            // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> >       _M_end_of_storage;    // +0x08
    size_t size() const { return size_t(_M_finish - _M_start); }
    size_t capacity() const { return size_t(_M_end_of_storage._M_data - _M_start); }
    pointer _M_allocate_and_copy(size_t __n, const _Tp* __first, const _Tp* __last);   // 0x0057F8A7
    __forceinline void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void reserve(size_t __n);
};

template <class _Tp>
void vector<_Tp>::reserve(size_t __n)
{
    if (capacity() < __n) {
        const size_t __old_size = size();
        pointer __tmp;
        if (_M_start) {
            __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            _M_clear();
        }
        else {
            __tmp = _M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template void vector<CHeroMarriageInfo>::reserve(size_t);

}