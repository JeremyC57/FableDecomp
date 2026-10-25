// std::vector::operator=(const vector& x) -- (vector<CBlendedEnvironmentTheme>, the NEnvironmentInternal theme) STLport assignment of 500-byte blended themes: reallocate-and-copy when x outgrows the capacity, else assign over the live prefix (destroying any surplus) or assign the prefix and copy-construct the tail; finish = start + x.size()
// Leaf: ??4?$vector@VCBlendedEnvironmentTheme@NEnvironmentInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889F00, 323 bytes, thiscall, `ret 4`, returns this.
// Identity: the catalog's "Vector_Reallocate" is descriptive. The 0x1F4 stride (`imul 0x10624dd3 ; sar 5`) and the
//   callees are the NEnvironmentInternal::CBlendedEnvironmentTheme vector's: __uninitialized_copy 0x00889880 (the landed
//   source lists 0x00889F72 / 0x0088A027 in this body as its callers), __destroy_aux 0x006B67A0 (landed, 500-byte theme
//   with three texture sets) and __copy 0x00889950 (landed, assigns through the theme's operator= 0x006B5DF0).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport malloc allocator), __uninitialized_copy(first, last, result,
//   const __false_type&) 0x00889880, __destroy_aux(first, last, const __false_type&) 0x006B67A0 and __copy(first, last,
//   result, const random_access_iterator_tag&, int*) 0x00889950 -- all out of line and /Gr `__fastcall` (first in ecx,
//   last in edx, the rest on the stack).
// Shape: STLport `vector::operator=` verbatim with _M_allocate_and_copy / _M_clear / __copy_ptrs / _Destroy inlined:
//   x._M_finish parked in the `push ecx` frame word, every empty tag homed in x's dead argument slot, the `push 0`
//   distance pointer of the two __copy calls hoisted above the `size() >= xlen` branch.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                            // 0x1F4 bytes in retail (three texture sets of CArrays)
public:
    unsigned char Data[0x1F4];
    CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme& other);                // 0x008894A0
    ~CBlendedEnvironmentTheme();                                                    // inlined into 0x006B67A0
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other);     // 0x006B5DF0
};
}

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

// Same-TU stand-ins for the out-of-line /Gr callees (real bodies, never inlined, tag parameters unnamed): VC7.1 then
// homes every empty tag on x's argument slot, as retail does, and re-reads x after the calls.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)               // 0x00889880
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) NEnvironmentInternal::CBlendedEnvironmentTheme(*__first);
    return __result;
}
template <class _ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)   // 0x006B67A0
{
    for (; __first != __last; ++__first)
        __first->~CBlendedEnvironmentTheme();
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)    // 0x00889950
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

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
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
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

template class vector<NEnvironmentInternal::CBlendedEnvironmentTheme>;

} // namespace std