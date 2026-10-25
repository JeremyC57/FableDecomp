// std::vector<unsigned short>::vector(const vector& x) -- STLport copy constructor of a 16-bit index vector: allocate exactly x.size() shorts (none when empty) and memmove the source range in
// Leaf: ??0?$vector@G
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A87090, 90 bytes, thiscall `ret 4`, returns this.
// Identity: 13 call sites in the 3D mesh/primitive code (0x00A86B21, 0x00A86BF6, 0x00A86C8F, 0x00A870F8, 0x00F3B755, ...);
//   the element is 2 bytes (`sar edi,1` size, `lea eax,[edi+edi]` allocation) and trivially copyable (memmove, no
//   per-element loop), i.e. the unsigned short index buffers the primitive builders copy. The catalog's
//   "Vector_Copy" is descriptive.
// Callees: CRT malloc 0x00BFEA0E (allocator::allocate, null for n == 0), CRT memmove 0x00BFEAE6.
// Shape: STLport 4.x verbatim: `_Vector_base(x.size(), x.get_allocator())` (size computed first, the three words zeroed
//   from one `xor eax,eax` that also serves as the null allocation), start/finish/end set, then `_M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, __true_type())` -> `__copy_trivial`
//   (`last == first ? result : memmove(result, first, n) + n`).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace std {

struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __true_type());
    }
};

template class vector<unsigned short>;
}