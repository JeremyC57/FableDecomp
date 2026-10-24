// std::vector<ENavigatorType>::operator=(const vector<ENavigatorType>& x) -- STLport vector assignment: reallocate and copy when x outgrows the capacity, else copy over the live elements and copy-construct the rest; returns *this
// Leaf: ??4?$vector@W4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006714B0, 231 bytes (+ int3 padding to 240), thiscall `ret 4`. Only caller:
//   CThingCreatureDef::operator= @ 0x00671160 (call at 0x0067126B, NavigatorTypes; the landed source already names this
//   callee vector<ENavigatorType>::operator=).
// Callees: _M_allocate_and_copy<const ENavigatorType*> @ 0x006715A0, _M_clear @ 0x006715F0 (landed under the stub name
//   "CActiveFile::OnReadFinished").
// Label correction: the catalog "CActiveFile_Assign" is a propagated name.
// Shape: STLport 4.x verbatim: `if (&x != this) { xlen = x.size(); if (xlen > capacity()) { tmp = _M_allocate_and_copy(xlen,
//   x.start, x.finish); _M_clear(); start = tmp; eos = start + xlen; } else if (size() >= xlen) { i = __copy_ptrs(x.start,
//   x.finish, start, __false_type()); _Destroy(i, finish); } else { __copy_ptrs(x.start, x.start + size(), start, ...);
//   __uninitialized_copy(x.start + size(), x.finish, finish, __false_type()); } finish = start + xlen; } return *this;`
//   (__copy_ptrs is the counted random-access __copy for a non-POD enum); the copy helpers are __forceinline (plain
//   inline leaves the three helper calls). No pragma.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum: STLport treats it as non-POD (__false_type)

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
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
template <class _Tp>
__forceinline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIter>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIter __first, _ForwardIter __last)
    ;                                                      // 0x006715A0
    void _M_clear();                                       // 0x006715F0
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
};
}

namespace std {
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
template class vector<ENavigatorType>;
}