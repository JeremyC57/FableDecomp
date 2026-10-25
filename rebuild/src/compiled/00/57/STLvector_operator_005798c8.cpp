// _STL::vector::operator=(const vector& x) -- STLport copy assignment of vector<NUISystem::CComponent*>: reallocate and copy when x outgrows the capacity, else memmove over the live prefix (and the uninitialised tail)
// Leaf: ??4?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@_STL@@@_STL@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005798C8, 144 bytes, __fastcall, `ret 4`, returns `*this`; size-optimised TU
//   with an ebp frame (no alignment padding).
// Identity: the out-of-line _M_allocate_and_copy it calls (landed 0x005796D1) is vector<NUISystem::CComponent*>'s; this
//   assignment is applied to CTCInventoryExperience::ExpSliders (callers 0x0057740B, 0x00577E49 in the same TU).
//   Label correction: the catalog's "vector<unsigned long>::operator=" names a folded 4-byte instantiation.
// Callees: vector::_M_allocate_and_copy(n, first, last) @ 0x005796D1 (out of line), free @ 0x00BFEA14 (the allocator's
//   deallocate in _M_clear), STLport __copy_trivial(first, last, result) @ 0x00405BA0 (__fastcall memmove helper, out
//   of line, x2 call sites).
// Shape: STLport 4.6 _vector.c verbatim (as in the landed vector<unsigned short>::operator= 0x00AADE80): the three-way
//   size test, `_M_clear()` = the empty POD _Destroy plus the null-tested free, `__copy_ptrs(..., __true_type)` going to
//   __copy_trivial; VC7.1 homes x._M_finish in x's dead argument slot and tail-merges the two copy calls.
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                               // 0x00BFEA14

namespace NUISystem { class CComponent; }

namespace _STL {

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

void* __fastcall __copy_trivial(const void* __first, const void* __last, void* __result);   // 0x00405BA0

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
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x005796D1
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

template vector<NUISystem::CComponent*>& vector<NUISystem::CComponent*>::operator=(const vector<NUISystem::CComponent*>&);

}   // namespace _STL