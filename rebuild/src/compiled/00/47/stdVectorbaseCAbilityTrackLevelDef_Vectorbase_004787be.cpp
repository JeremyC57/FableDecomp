// std::_Vector_base<CAbilityTrackLevelDef>::_Vector_base(size_t n, const allocator& a) -- STLport vector storage constructor: zero the three words, allocate n 0x48-byte elements, set start/finish/end
// Retail: Fable.exe (TLC, VC7.1) @ 0x004787BE, 56 bytes, `ret 8` (returns this).
// Callees: malloc @ 0x00BFEA0E (the STLport allocator's allocate: `n != 0 ? malloc(n * sizeof(T)) : 0`).
// Shape: STLport 4.x `_Vector_base(size_t __n, const _Alloc& __a) : _M_start(0), _M_finish(0),
// _M_end_of_storage(__a, 0) { _M_start = _M_end_of_storage.allocate(__n); _M_finish = _M_start;
// _M_end_of_storage._M_data = _M_start + __n; }` -- the end-of-storage word is an `_STLP_alloc_proxy` over the
// empty allocator. One zero register clears all three words before the n == 0 test; the 0x48 scaling is
// `lea [n+n*8] ; shl 3` for the size and `lea ecx,[n+n*8] ; lea eax,[eax+ecx*8]` for the end pointer, which
// this time is stored after start/finish (same source as the 0x88-byte sibling 0x00477530). Pragma: optimize("s") (`pop ecx` after malloc); the size TU inlines the
// allocator's allocate and the proxy constructor only as `__forceinline`.
// Label: the catalog's "CAbilityTrackLevelDef_vector_Reserve" (caller: the vector copy constructor 0x0047876B).
// Leaf: ??0?$_Vector_base
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

struct CAbilityTrackLevelDef {
    unsigned char _body[0x48];
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    __forceinline _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a);
};

template <class _Tp, class _Alloc>
_Vector_base<_Tp, _Alloc>::_Vector_base(unsigned int __n, const _Alloc& __a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
{
    _M_start = _M_end_of_storage.allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + __n;
}

template class _Vector_base<CAbilityTrackLevelDef, allocator<CAbilityTrackLevelDef> >;

}