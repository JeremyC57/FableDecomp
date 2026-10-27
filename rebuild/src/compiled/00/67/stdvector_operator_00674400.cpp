// std::vector::operator=(const vector& __x) -- (vector<std::pair<EExpressionType, CExpressionSet::CEntry> >) STLport assignment: if __x is not *this, grow into a fresh block when __x is larger than the capacity, else assign over the live elements and copy-construct the tail; size becomes __x.size()
// Leaf: ??4?$vector@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@V?$allocator@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00674400, 276 bytes, thiscall, returns *this, `ret 4`.
// Identity (proven): the landed CCreatureDef::operator= 0x00675C00 calls it for CExpressionSet::Expressions (Ego_r.pdb CVectorMap<EExpressionType, CEntry, ...>); its _M_allocate_and_copy 0x00674030 and __copy 0x00672F40 are landed as the pair<EExpressionType, CExpressionSet::CEntry> instantiations (proven there).
// Callees: vector::_M_allocate_and_copy<const T*>(n, first, last) 0x00674030, free 0x00BFEA14,
//   __copy<const T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x00672F40 and
//   __uninitialized_copy<const T*, T*>(first, last, result, const __false_type&) 0x00673CF0 (__fastcall).
// Shape (family M187, representative 0x006741E0): STLport 4.6 vector.c operator= verbatim (the landed L032 model 0x006755E0)
//   over a trivially destructible 12-byte element: _M_clear and the surplus _Destroy reduce to `if (_M_start) free(_M_start)`
//   and nothing; _M_allocate_and_copy stays out of line; __copy / __uninitialized_copy have same-TU noinline bodies so the
//   empty tags home on the __x slot. Speed TU.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
enum EExpressionType { EXPRESSION_TYPE_NONE = 0 };
class CExpressionSet {
public:
    class CEntry {                                         // Ego_r.pdb, 8 bytes
    public:
        EExpressionType Type;                             // +0x00
        long AnimIndex;                                   // +0x04
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)        // 0x00672F40
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)  // 0x00673CF0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
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
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00674030
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
template vector<std::pair<EExpressionType, CExpressionSet::CEntry> >& vector<std::pair<EExpressionType, CExpressionSet::CEntry> >::operator=(const vector<std::pair<EExpressionType, CExpressionSet::CEntry> >&);
}