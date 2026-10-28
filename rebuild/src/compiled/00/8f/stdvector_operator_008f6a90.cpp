// std::vector::operator=(const vector& x) -- (vector<CHeightMapCell>, CHeightMap::Cells) STLport assignment of 12-byte height-map cells: reallocate-and-copy when x outgrows the capacity, else assign over the live prefix, or assign the prefix and copy-construct the tail; finish = start + x.size(); returns *this
// Leaf: ??4?$vector@VCHeightMapCell@@V?$allocator@VCHeightMapCell@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F6A90, 281 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): the only caller is CHeightMap::Resize 0x008F61A0 (call at 0x008F6368; Ego_r.pdb
//   ?Resize@CHeightMap@@AAEXJJJJ@Z), which assigns a rebuilt grid to Ego_r.pdb CHeightMap::Cells (vector<CHeightMapCell>,
//   12-byte cells); its helpers are the landed CHeightMapCell __copy 0x008F6630 and __uninitialized_copy 0x008F67C0.
//   The catalog's "std_vector_assign<CTCSmokeGenerator::CGenerationPos>" is propagated (CGenerationPos is 20 bytes).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport malloc allocator), __uninitialized_copy<const CHeightMapCell*,
//   CHeightMapCell*> 0x008F67C0 and
//   __copy<const CHeightMapCell*, CHeightMapCell*, int> 0x008F6630, both /Gr `__fastcall`.
// Model: the landed byte-shape twin 0x00888EE0 (STLport vector::operator= verbatim, same-TU noinline helper bodies so the
//   empty tags home on x's dead argument slot). Naming: STLport 4.6 operator= passes the source as const_pointer, so
//   0x008F67C0 is the <const CHeightMapCell*, CHeightMapCell*> instantiation; the <CHeightMapCell*, CHeightMapCell*> one
//   is its byte twin 0x008F6800 (bound by the landed _M_fill_insert 0x008F6C90). Landed 0x008F67C0 carries the PAV
//   spelling and needs the PBV one (label correction).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

#include "engine/CHeightMapCell.h"

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

// Same-TU stand-ins for the out-of-line /Gr callees (real bodies, never inlined, tag parameters unnamed): VC7.1 then
// homes every empty tag on x's argument slot, as retail does, and re-reads x after the calls.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)               // 0x008F67C0
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) CHeightMapCell(*__first);
    return __result;
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)    // 0x008F6630
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
        __first->~CHeightMapCell();
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
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
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
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CHeightMapCell>& vector<CHeightMapCell>::operator=(const vector<CHeightMapCell>&);

} // namespace std