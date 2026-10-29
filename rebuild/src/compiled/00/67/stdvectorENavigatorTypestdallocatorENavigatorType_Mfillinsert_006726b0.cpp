// std::vector<ENavigatorType>::_M_fill_insert -- original typed navigator insertion.
// Leaf: ?_M_fill_insert@?$vector@W4ENavigatorType@@V?$allocator@W4ENavigatorType@@@std@@@std@@QAEXPAW4ENavigatorType@@IABW43@@Z
// Flags:
#include <stddef.h>
#include <new>
#include "rebuild_abi.h"
enum ENavigatorType {};
namespace std {
struct __true_type {};struct __false_type {};
template<class T>struct __type_traits {typedef __true_type this_dummy_member_must_be_first;typedef __false_type has_trivial_default_constructor,has_trivial_copy_constructor,has_trivial_assignment_operator,has_trivial_destructor,is_POD_type;};
struct input_iterator_tag {};struct output_iterator_tag {};struct forward_iterator_tag:public input_iterator_tag {};struct bidirectional_iterator_tag:public forward_iterator_tag {};struct random_access_iterator_tag:public bidirectional_iterator_tag {};

template <class _T1, class _T2> __forceinline void __fastcall _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template<class _InputIter,class _OutputIter>__forceinline _OutputIter __fastcall __copy_backward_ptrs(_InputIter first,_InputIter last,_OutputIter result,const __false_type&){return __copy_backward(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);}

template <class _ForwardIter, class _Tp>
__forceinline void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template<class F,class N,class T>__forceinline F __fastcall __uninitialized_fill_n(F first,N n,const T&x,const __false_type&){F cur=first;for(;n>0;--n,++cur)_Construct(&*cur,x);return cur;}
#pragma auto_inline(off)
template<class F,class N,class T>__declspec(noinline) F __fastcall uninitialized_fill_n(F first,N n,const T&x);
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x00672820
public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<ENavigatorType>::_M_fill_insert(ENavigatorType*, unsigned int, const ENavigatorType&);
}