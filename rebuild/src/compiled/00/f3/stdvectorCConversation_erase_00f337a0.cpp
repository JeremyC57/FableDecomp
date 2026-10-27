// std::vector<CConversation>::erase(iterator first, iterator last) -- STLport range erase: assign the tail [last, finish) down onto first through the element's operator=, destroy the vacated elements through their virtual destructor, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCConversation@@V?$allocator@VCConversation@@@std@@@std@@QAEPAVCConversation@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f337a0, 109 bytes, thiscall, `ret 8`, returns first.
// Identity (proven): its only caller is the landed resize 0x00F33730 (a CRetailVector stand-in, 92-byte elements); the element assignment 0x00E54C50 is the landed CConversation::operator=; the growth path 0x00F33A10 copy-constructs through the bound ??0CConversation@@QAE@ABV0@@Z.
// Callees: CConversation::operator=(const CConversation&) 0x00E54C50; the element's virtual destructor (vtable slot 0, flags 0).
// Shape (family L022 = family L001 with a non-trivial element): STLport 4.6 erase verbatim; __copy_ptrs / __copy
//   __forceinline (plain `inline` leaves them out of line), the _Destroy loop of `p->~T()` virtual calls inline.
#include <stddef.h>
class CConversation {                                              // retail 92 bytes: vptr + def data (not touched here)
public:
    virtual ~CConversation();                                      // slot 0 (the scalar deleting destructor)
    CConversation& operator=(const CConversation& other);                  // 0x00E54C50
    unsigned char Data[92 - 4];
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
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
template <class _Tp>
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}
template CConversation* std::vector<CConversation>::erase(CConversation*, CConversation*);