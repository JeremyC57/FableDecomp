// std::vector<unsigned short>::operator=(const vector& x) -- STLport copy assignment of a 16-bit vector: reallocate and copy when x outgrows the capacity, else memmove over the live prefix (and the uninitialised tail)
// Leaf: ??4?$vector@G
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AADE80, length 236 bytes, `ret 4`.
// Callees: malloc @ 0x00BFEA0E (allocate), memmove @ 0x00BFEAE6 (STLport __copy_trivial), free @ 0x00BFEA14
//   (deallocate in _M_clear).
// Identity: the 2-byte stride (`sar r,1`, `lea [x+n*2]`) and the trivial-copy POD paths make this
//   vector<unsigned short>::operator=, the partner of the vector<unsigned short>(n) constructor 0x00AADD80 just above.
//   The manifest's "Vector_Assign_uint16" is descriptive.
// Shape: STLport 4.6 verbatim -- `_M_allocate_and_copy` inlined (allocate, `__uninitialized_copy` with __true_type =
//   `__copy_trivial`'s `last == first ? result : memmove(...) + n` ternary), `_M_clear` = the empty POD _Destroy plus the
//   null-tested free, the three-way size test, the `__copy_ptrs(..., __true_type)` memmoves; VC7.1 homes the new block
//   in x's dead argument slot and tail-duplicates the final `_M_finish = _M_start + xlen`.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                               // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);     // 0x00BFEAE6

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __true_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    vector& operator=(const vector& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __true_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __true_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __true_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<unsigned short>& vector<unsigned short>::operator=(const vector<unsigned short>&);

}   // namespace std