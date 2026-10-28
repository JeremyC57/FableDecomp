// std::vector::operator=(const vector& x) -- (vector of a 12-byte three-dword record, descriptive SElement12) STLport assignment: reallocate-and-copy when x outgrows the capacity, else assign over the live prefix or assign the prefix and copy-construct the tail; finish = start + x.size()
// Leaf: ??4?$vector@USElement12@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00888EE0, 281 bytes, thiscall, `ret 4`, returns this.
// Identity: the catalog's "Vector_Reallocate" is descriptive. The 12-byte stride (`imul 0x2aaaaaab ; sar 1`) and the
//   callees name the element only by shape: it is the 12-byte member at +8 of the 48-byte record copied by the landed
//   __copy 0x00889650 / __copy_backward 0x008893D0 / fill 0x008895F0 of the environment TU (all call this body as the
//   member's operator=), and 0x00889290 wraps it as a container operator= returning this. No PDB name fits a
//   three-dword record with an implicit (trivial) destructor here, so the element is modelled by its size.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport malloc allocator), __uninitialized_copy(first, last, result,
//   const __false_type&) 0x006B56F0 (placement-copy loop, folded with other 12-byte instantiations) and __copy(first,
//   last, result, const random_access_iterator_tag&, int*) 0x00885ED0 (landed) -- out of line, /Gr `__fastcall`.
// Shape: STLport `vector::operator=` verbatim with _M_allocate_and_copy / _M_clear / __copy_ptrs / _Destroy inlined; the
//   element's destructor is trivial, so both _Destroy loops vanish (no __destroy_aux call). x._M_finish parked in the
//   `push ecx` frame word, the empty tags homed in x's dead argument slot (same-TU noinline stand-ins), the `push 0`
//   distance pointer of the two __copy calls hoisted above the `size() >= xlen` branch.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct SElement12 {                                         // 12 bytes, copied as three dwords; trivial destructor
    long m_words[3];
    SElement12() { }
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

// Same-TU stand-ins for the out-of-line /Gr callees (real bodies, never inlined, tag parameters unnamed): VC7.1 then
// homes every empty tag on x's argument slot, as retail does, and re-reads x after the calls.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)               // 0x006B56F0
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) SElement12(*__first);
    return __result;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)    // 0x00885ED0
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _ForwardIter>
inline void __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        __first->~SElement12();
}
template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

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
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;

    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
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

template class vector<SElement12>;

} // namespace std