// std::vector<CWideString>::operator=(const vector& __x) -- STLport 4.6 copy assignment: reallocate and copy-construct when the source is longer than the capacity, else assign over the live elements and destroy the surplus, or assign the overlap and copy-construct the tail
// Leaf: ??4?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00433DF0, 369 bytes, thiscall `ret 4`, speed TU (16-byte aligned entry, esp frame).
// Identity (proven): std::vector<CWideString>::operator=(const vector&); evidence: (1) the body is STLport 4.6 _vector.c operator=
//   verbatim for a 4-byte class element: the realloc arm mallocs xlen*4 (0x00BFEA0E), copy-constructs with the CWideString copy
//   constructor 0x0099B720 behind the placement-new null test, destroys the old elements with ~CWideString 0x0099B510 and frees
//   the block (0x00BFEA14); the shrink arm assigns with CWideString::operator= 0x0099B7D0 then destroys the surplus; the grow arm
//   assigns the overlap and calls std::__uninitialized_copy<const CWideString*, CWideString*> 0x00433D8A (the same copy-ctor loop);
//   (2) the Ego_r/FableWin.pdb name sets hold ??4?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEAAV01@ABV01@@Z
//   (STLport and the PDB's Dinkumware spell the class template identically); (3) its callers (0x00433C68 / 0x00433C74,
//   0x0044E91D / 0x0044E929, 0x0047E18E, 0x00CD4217 / 0x00CD4224) assign vector<CWideString> members (CDefinitionManager
//   SetCompilePathList / SetSymbolPathList take `const vector<CWideString>&`).
// Label: the catalog's "Array_AssignFrom" is behavioural; the binding ??4CWideStringVector@@QAEAAV0@ABV0@@Z is an invented class.
// Callees: ??0CWideString@@QAE@ABV0@@Z 0x0099B720, ??1CWideString@@QAE@XZ 0x0099B510, ??4CWideString@@QAEABV0@ABV0@@Z 0x0099B7D0
//   (PDB: returns a const reference), malloc 0x00BFEA0E, free 0x00BFEA14, std::__uninitialized_copy 0x00433D8A (__fastcall).
// Shape: STLport 4.6 operator= in a /Gr speed TU (the out-of-line __uninitialized_copy 0x00433D8A is __fastcall). The loop-holding
//   helpers retail inlined (_M_allocate_and_copy with its copy loop, _M_clear, __copy_ptrs / __copy, the range _Destroy) need
//   __forceinline under VC7.1; __uninitialized_copy stays plain inline, so the grow arm calls it out of line with the
//   __false_type tag homed in the realloc arm's `__tmp` slot, as retail does.
// Flags: /Gr
#include <stdlib.h>

class CWideString {                                      // Ego_r.pdb, 4 bytes (refcounted wide-string data pointer)
public:
    void* PStringData;
    CWideString(const CWideString& other);               // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

inline void* operator new(size_t, void* __p) { return __p; }

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
__forceinline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                          _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef size_t size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    __forceinline void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    template <class _ForwardIterator>
    __forceinline pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CWideString, allocator<CWideString> >& vector<CWideString, allocator<CWideString> >::operator=(const vector<CWideString, allocator<CWideString> >&);
}