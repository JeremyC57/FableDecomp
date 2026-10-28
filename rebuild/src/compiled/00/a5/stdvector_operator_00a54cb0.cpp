// std::vector::operator=(const vector& x) -- (vector<C3DAnimationSequenceData::C3DVectorWord>) STLport assignment: if x is not *this, grow into a fresh block (_M_allocate_and_copy, free the old one) when x is larger than the capacity, else assign over the live elements or copy-construct the tail; size becomes x.size()
// Leaf: ??4?$vector@VC3DVectorWord@C3DAnimationSequenceData@@V?$allocator@VC3DVectorWord@C3DAnimationSequenceData@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A54CB0, 268 bytes, thiscall, returns *this, `ret 4`.
// Identity: Ego_r.pdb ??4?$vector@VC3DVectorWord@C3DAnimationSequenceData@@...@QAEAAV01@ABV01@@Z. Callers: four sites in
//   0x00A4EFC0 (the C3DAnimationSequenceData compressor: it sets PositionTrackMode, bits 2-3 of byte +0x0B, right before
//   each assignment of a local position-track vector) and the CArray<C3DVectorWord>::operator= forwarder 0x00A54DE0.
//   The element is 6 bytes (0x2AAAAAAB, no shift) = Ego_r C3DAnimationSequenceData::C3DVectorWord {short X, Y, Z}
//   (class key V; C3DAnimationSequenceData::PositionTrack is a C3DVectorWord*). The catalog's
//   "Array_C3DVectorWord_Assign" is descriptive.
// Callees: _M_allocate_and_copy(n, first, last) 0x00A53650 (thiscall, out of line; the landed source calls the element
//   C3DTriangle2 -- NAME_FIXES), free 0x00BFEA14 (_M_clear's deallocate; the trivial _Destroy vanishes),
//   __copy(first, last, result, const random_access_iterator_tag&, int*) 0x00A52260 and __uninitialized_copy(first, last,
//   result, const __false_type&) 0x00A52040 (both /Gr __fastcall).
// Shape: STLport 4.6 _vector.c operator= verbatim (landed twin 0x00A46790); the empty tags are homed on the x argument
//   slot because __copy / __uninitialized_copy have same-TU noinline bodies.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class C3DAnimationSequenceData {
public:
    class C3DVectorWord {                                  // Ego_r.pdb, 6 bytes
    public:
        short X;                                           // +0x00
        short Y;                                           // +0x02
        short Z;                                           // +0x04
    };
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)        // 0x00A52260
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)  // 0x00A52040
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00A53650
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
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
}
template std::vector<C3DAnimationSequenceData::C3DVectorWord>& std::vector<C3DAnimationSequenceData::C3DVectorWord>::operator=(const std::vector<C3DAnimationSequenceData::C3DVectorWord>&);