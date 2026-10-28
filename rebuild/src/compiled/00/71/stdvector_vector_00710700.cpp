// std::vector::vector(const vector& x) -- (vector<short>) STLport copy constructor: allocate x.size() shorts (none for an empty source), set start/finish/end of storage, memmove the elements in when there are any
// Leaf: ??0?$vector@FV?$allocator@F@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00710700, 90 bytes, thiscall, `ret 4` (&x), returns this.
// Identity (proven): the only caller is the landed CAnimSet::CAnimSet(const CAnimSet&) 0x00710B80, which copies
//   Ego_r.pdb CAnimSet::Anims (vector<short, allocator<short> >, +0x04) through this body (`sar edi,1`: 2-byte
//   elements). Ego_r.pdb ??0?$vector@FV?$allocator@F@std@@@std@@QAE@ABV01@@Z. The catalog label
//   "CAnimSet::CopyConstruct_Shorts" is descriptive; the landed caller binds this address as an invented
//   CVectorShort::CVectorShort(const CVectorShort&) (see NOTES "Review at landing").
// Callees: malloc 0x00BFEA0E (allocator::allocate, skipped for n == 0), memmove 0x00BFEAE6 (__copy_trivial).
// Shape: STLport 4.6 `vector(const _Self& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, _IsPODType()); }`; short is POD, so the copy is
//   __copy_trivial (`last == first ? result : memmove(...) + n`).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                                        // +0x0
    _Tp* _M_finish;                                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;               // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start,
                                               __true_type());
    }
};
}

template std::vector<short>::vector(const std::vector<short>&);