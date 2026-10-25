// _STL::vector<CIntelligentPointer<CThing> >::_M_fill_insert(iterator position, size_type n, const CIntelligentPointer<CThing>& x) -- STLport fill insert of n copies of a thing smart pointer: shift the tail in place when capacity allows, else reallocate through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006562E7, length 232 bytes, `ret 0Ch` (thiscall: position, n, &x).
// Callers: 0x0065686E (tail jmp from insert(position, n, x)), 0x00656D54, 0x00739B9E.
// Callees: CIntelligentPointer<CThing> copy constructor @ 0x004A9730 (landed; the x_copy local),
//   __uninitialized_copy(first, last, result, const __false_type&) @ 0x004AA140, __copy_backward(first, last, result,
//   const random_access_iterator_tag&, int*) @ 0x00655884, fill(first, last, const T&) @ 0x0065572D (landed under the
//   propagated name "_Destroy_range"; its loop calls operator= 0x004A9760 with &val), __uninitialized_fill_n(first, n,
//   const T&, const __false_type&) @ 0x004AA190, ~CBaseIntelligentPointer @ 0x00A01C10 (x_copy's destructor: the
//   class declares none, so the generated one only calls the base), _M_insert_overflow(position, x, const
//   __false_type&, n, false) @ 0x004AC940.
// Label: catalog "std::vector_insert_animationentry" is descriptive (named after the mislabelled fill callee).
// Shape: STLport 4.6 `_vector.c` `_M_fill_insert` verbatim (exceptions off). Size-optimised frame-pointer TU
//   (`push ebp ; mov ebp,esp`, unaligned entry: `optimize("y", off)` + `("s", on)`), so the inline forwarders
//   (uninitialized_fill_n, __copy_backward_ptrs) are `__forceinline`; every tag temporary shares the high byte of the
//   live `position` slot (`lea [ebp+0Bh]`) -- that needs same-TU noinline bodies of the three tag-taking helpers (with them
//   declaration-only the __false_type tags move to the `n` slot and the parked values to the `x` slot); `__n -
//   __elems_after` and `__old_finish - __n` are parked in the dead `n` slot.
struct CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* m_pItem;                                    // +0x04
    ~CBaseIntelligentPointer();                              // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other);                 // 0x004A9730
    CIntelligentPointer<T>& operator=(const CIntelligentPointer<T>& other);   // 0x004A9760
};

inline void* operator new(unsigned int, void* __p) { return __p; }

namespace _STL {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator { };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU noinline bodies of the three out-of-line tag-taking helpers (STLport bodies; none reads its tag), so VC7.1
// knows the tag temporaries are never touched and homes them all in the live `position` slot as retail.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                               // 0x004AA140
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                                                    const random_access_iterator_tag&, _Distance*)
{                                                                               // 0x00655884
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                               // 0x004AA190
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);   // 0x0065572D

template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                           // +0x00
    _Tp* _M_finish;                                          // +0x04
    _Tp* _M_end_of_storage;                                  // +0x08 (_STLP_alloc_proxy over the empty allocator)
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);   // 0x004AC940
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

typedef CIntelligentPointer<CThing> TThingPtr;
template TThingPtr* __fastcall _STL::__uninitialized_copy<TThingPtr*, TThingPtr*>(TThingPtr*, TThingPtr*, TThingPtr*, const _STL::__false_type&);
template TThingPtr* __fastcall _STL::__copy_backward<TThingPtr*, TThingPtr*, int>(TThingPtr*, TThingPtr*, TThingPtr*, const _STL::random_access_iterator_tag&, int*);
template TThingPtr* __fastcall _STL::__uninitialized_fill_n<TThingPtr*, unsigned int, TThingPtr>(TThingPtr*, unsigned int, const TThingPtr&, const _STL::__false_type&);
#pragma optimize("y", off)
#pragma optimize("s", on)
template void _STL::vector<CIntelligentPointer<CThing> >::_M_fill_insert(CIntelligentPointer<CThing>*, unsigned int, const CIntelligentPointer<CThing>&);