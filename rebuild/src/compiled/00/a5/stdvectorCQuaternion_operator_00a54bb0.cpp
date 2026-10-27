// std::vector<CQuaternion>::operator=(const vector& x) -- STLport assignment: if x is not *this, grow into a fresh block (malloc, copy-construct, free the old one) when x is larger than the capacity, else assign over the live elements and copy-construct the tail; size becomes x.size()
// Leaf: ??4?$vector@VCQuaternion@@V?$allocator@VCQuaternion@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a54bb0, 253 bytes, thiscall, returns *this, `ret 4`.
// Identity (probable): Ego_r.pdb ??4?$vector@VCQuaternion@@...@QAEAAV01@ABV01@@Z is its only 16-byte-element vector assignment in the animation code; both calls in C3DAnimationSequenceData::InitialiseFromChunk 0x00A4EFC0 come right after it sets C3DAnimationSequenceData::RotationTrackMode (bits 0-1 of byte +0x0B), and 0x00A54DC0 is a forwarding operator= over the same vector.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __copy_ptrs<T>(first, last, result, const __false_type&) 0x00A535F0 and
//   __uninitialized_copy<const T*, T*>(first, last, result, const __false_type&) 0x00A51FA0 (both __fastcall, out of line).
// Shape (family L030): STLport 4.6 vector.c operator= verbatim (trivial destructors fold _Destroy away); the empty tags
//   are homed on the x argument slot, which VC7.1 does only when the two tag-taking helpers have same-TU noinline
//   bodies (their real STLport bodies here; only operator= is graded).
class CQuaternion {                                       // Ego_r.pdb, 16 bytes
public:
    float X, Y, Z, W;                                     // +0x00
};
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result,
                                                        const __false_type&)                     // 0x00A535F0
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)  // 0x00A51FA0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
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
template std::vector<CQuaternion >& std::vector<CQuaternion >::operator=(const std::vector<CQuaternion >&);