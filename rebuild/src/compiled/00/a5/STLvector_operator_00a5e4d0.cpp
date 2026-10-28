// std::vector::operator=(const vector& x) -- STLport vector assignment for an 8-byte trivially-copyable element (SElement8, descriptive): reallocate-and-copy when x outgrows the capacity, else copy over the live prefix and copy-construct any tail; the finish pointer set to start + x.size()
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5E4D0, 198 bytes, `ret 4`.
// Callers: 0x00A5E618 (a CArray<> operator= wrapper @ 0x00A5E610) and the implicit copy assignments @ 0x00A5E730 /
//   0x00A5E790 / 0x00A5E830 of three classes holding such an array. Callees: vector::_M_allocate_and_copy @ 0x00A5DEE0,
//   free @ 0x00BFEA14 (_M_clear's deallocate), __copy_ptrs(first, last, result, __false_type) @ 0x00A5DF30 and
//   __uninitialized_copy(first, last, result, __false_type) @ 0x00A5DDE0 (both out of line, __fastcall: first ecx,
//   last edx, then the result and the tag reference on the stack).
// Label: the manifest's `StdVector_Assign_PairFloatEnum_v2` is propagated from an Ego_r Dinkumware body; the retail element
//   type is not named by any table (an 8-byte struct copied as two dwords). STLport 4.6 `vector::operator=`.
// Shape: the __false_type tag (user ctor: no store) is homed in x's dead argument slot (x lives in ebp).
#include <stddef.h>

extern "C" void __cdecl free(void* p);          // 0x00BFEA14

struct SElement8 {                              // 8 bytes, copied memberwise as two dwords
    long m_00;
    long m_04;
};

namespace std {

struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&);   // 0x00A5DF30
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                                               // 0x00A5DDE0

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef size_t size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;

    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00A5DEE0
    public:
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
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
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish,
                                 _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template class vector<SElement8>;

}   // namespace std