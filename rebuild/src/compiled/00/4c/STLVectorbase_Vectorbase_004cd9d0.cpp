// std::_Vector_base::_Vector_base(unsigned int n, const allocator<unsigned long>& a) -- STLport vector base for 4-byte elements: null the three pointers, allocate room for n elements, finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD9D0, 56 bytes, thiscall `ret 8` (n and the allocator reference).
// Caller: 0x0057FCEB, the vector<unsigned long>(size_type n, const unsigned long& value) constructor at 0x0057FCDD
//   (manifest "std::vector<ulong>::vector"; it fills the block through 0x0044B150 afterwards). The body is one COMDAT
//   per 4-byte element type, folded; byte twin of the landed _Vector_base<void*> 0x0050FA40.
// Callee: malloc 0x00BFEA0E (`__cdecl`, result kept: `add esp,4`).
// Shape: STLport `_Vector_base(n, a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { _M_start =
//   _M_end_of_storage.allocate(n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + n; }` with the
//   end-of-storage pointer in an `_STLP_alloc_proxy` over the empty allocator; `allocate(n)` = `n != 0 ? malloc(n * 4) : 0`
//   reuses the shared zero register for the null block. The catalog's "std::vector_ctor" is descriptive.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template class _Vector_base<unsigned long, allocator<unsigned long> >;

}