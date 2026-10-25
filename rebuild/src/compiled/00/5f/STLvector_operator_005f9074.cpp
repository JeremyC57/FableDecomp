// _STL::vector::operator=(const vector<pair<long, EHeroAbility> >& x) -- assign the hero ability-button map's entry vector: reallocate and copy when x outgrows the capacity, else copy over the live elements and copy-construct the rest
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9074, 169 bytes, thiscall `ret 4`, in a size-optimised TU (unaligned entry, ebp frame).
// Callers: 0x005F9BBB -- CVectorMap<long, EHeroAbility>::operator= 0x005F9BB2 (the map's vector, then Dirty), which the landed
//   CTCInventoryAbilities::Copy 0x005F3DC6 calls for AbilityButtons; and 0x005F942E.
// Callees: _M_allocate_and_copy(n, first, last) 0x005F8BD8, free 0x00BFEA14, __copy(first, last, result,
//   random_access_iterator_tag, ptrdiff_t*) 0x005F7C3A, __uninitialized_copy(first, last, result, __false_type) 0x005F8325 (landed).
// Element: STLport pair<long, EHeroAbility> (8 bytes); a struct, so the type traits give __false_type: element-wise __copy and
//   placement-copy __uninitialized_copy.
// Label: catalog "vector<unsigned int>::operator=" is propagated.
// Shape: STLport 4.6 `operator=` verbatim (`_M_allocate_and_copy` + `_M_clear` + `_M_start/_M_end_of_storage` when xlen >
//   capacity(); `__copy_ptrs` + _Destroy when size() >= xlen; else `__copy_ptrs` over size() then `__uninitialized_copy` of
//   the rest; `_M_finish = _M_start + xlen`). The empty tag temporaries share the dead byte at [ebp+0Bh]; the common
//   `(ptrdiff_t*)0` push is hoisted above the size test. `#pragma optimize("s", on)` stands for the TU's size optimisation.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* p);          // 0x00BFEA14

enum EHeroAbility { HERO_ABILITY_NULL };

namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x0
    _T2 second;                                 // +0x4
};

template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                     // 0x005F7C3A
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                              // 0x005F8325

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x005F8BD8
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0,
                                      (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                        __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish,
                                 __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<pair<long, EHeroAbility> >& vector<pair<long, EHeroAbility> >::operator=(const vector<pair<long, EHeroAbility> >&);
}