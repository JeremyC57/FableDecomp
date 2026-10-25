// std::vector::operator=(const vector& __x) -- (vector<CTextureReplacementDef::CEntry >) STLport assignment of 8-byte elements: reallocate when __x outgrows the capacity, else assign over the live elements and copy-construct the tail; returns *this
// Leaf: ??4?$vector@VCEntry@CTextureReplacementDef@@V?$allocator@VCEntry@CTextureReplacementDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00456263, 225 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 4`.
// Identity: std::vector<CTextureReplacementDef::CEntry >::operator= -- proven; evidence: its only caller is CTextureReplacementDef::Copy 0x00456244 (landed), assigning Entries at this+0x28 after the CDefClassBase part; the FableWin.pdb name set holds ??4?$vector@VCEntry@CTextureReplacementDef@@V?$allocator@VCEntry@CTextureReplacementDef@@@std@@@std@@QAEAAV01@ABV01@@Z.
//   Family G060 (seven vector assignments of 8-byte elements; catalogue "CTextureReplacementDef::CTextureReplacementDef::Copy" is behavioural).
// Callees: _M_allocate_and_copy<const T*>(n, first, last) 0x00456344 (thiscall, `ret 0Ch`), _M_clear() 0x00456389 (frees the block;
//   the element destructor is trivial).
// Shape: STLport 4.6 vector.c verbatim (as the landed vector<CRGBColour> 0x0045BAAA): __xlen homed in the dead __x slot,
//   the __copy loops counted down, the tail construct inline behind placement new's null test (/Os: the STLport
//   helpers need __forceinline to inline as retail does).
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }
class CTextureReplacementDef {
public:
    class CEntry { public: unsigned long Data[2]; };       // 8 bytes (fields not in Ego_r.pdb)
};
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
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
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;                      // __type_traits of a class element
    typedef __false_type _IsPODType;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00456344
    void _M_clear();                                                                                  // 0x00456389
};
template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            _M_start = __tmp;
            _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen) {
            __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)_M_start, _TrivialAss());
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)_M_start, _TrivialAss());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, _M_finish, _IsPODType());
        }
        _M_finish = _M_start + __xlen;
    }
    return *this;
}
typedef CTextureReplacementDef::CEntry _Elem;
template vector<_Elem>& vector<_Elem>::operator=(const vector<_Elem>&);
}