// std::vector::operator=(const vector<const CThing*>& x) -- STLport assignment of a vector of thing pointers (CTCFamilyMember::FamilyMembers): reallocate and memmove when x exceeds the capacity, else memmove over the live elements and memmove the tail
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778C00, length 256 bytes (thiscall, `ret 4`, returns this; row includes
//   the 0xCC pad to the 16-byte boundary).
// Identity: the catalog label "Vector_Assign" is descriptive. Its only caller is the CTCFamilyMember virtual 0x00778B10
//   (`jmp` tail call with this+0x0C and other+0x0C), and Ego_r.pdb CTCFamilyMember holds
//   `vector<const CThing*> FamilyMembers` at +0x0C, so the element is a 4-byte pointer (`sar 2`).
// Callees: malloc 0x00BFEA0E (allocator::allocate), memmove 0x00BFEAE6 (three sites: the allocate-and-copy, the shrink
//   copy and the shared tail of the grow-in-place copies), free 0x00BFEA14 (the inlined _M_clear's deallocate).
// Shape: STLport 4.x vector.c `operator=` verbatim over a POD element: _TrivialAss and _IsPODType are __true_type, so
//   __copy_ptrs and __uninitialized_copy both collapse to `__copy_trivial` (`last == first ? result :
//   memmove(result, first, last - first) + (last - first)`), `_Destroy(i, finish)` is empty and `_M_clear()` is just the
//   deallocate. All helpers inline; the allocated block is spilled into the dead `x` slot.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

class CThing;

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
                                     ((const char*)__last - (const char*)__first);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __true_type());
}

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
    typedef __true_type _TrivialAss;
    typedef __true_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0,
                                      (pointer)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                        _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0,
                                 this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<const CThing*>& vector<const CThing*>::operator=(const vector<const CThing*>&);

}