// std::vector<ExperienceSpendInfo>::_M_fill_insert(iterator pos, size_type n, const T& x) -- STLport insert of n copies of a 12-byte spend record, in place or through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00579B3C, 233 bytes, thiscall `ret 0Ch`, ebp frame (size TU). Called by the
//   vector's resize (landed 0x00579EF5, `insert(end(), n - size(), x)`) and 0x00579D55; the element is
//   CTCInventoryExperience::ExperienceSpendInfo (see 0x00578C50). The catalog "Vector_InsertRangeWithGrow" is
//   descriptive.
// Callees (all this TU's out-of-line STLport COMDATs): __uninitialized_copy(.., const __false_type&) @ 0x005790C3,
//   __copy_backward(.., random_access_iterator_tag, int*) @ 0x00578CF7, fill @ 0x00578C0F, __uninitialized_fill_n
//   @ 0x00578C8D, _M_insert_overflow @ 0x00579A4B (the last four rows of this chunk but fill).
// Shape: STLport 4.x verbatim: `if (__n != 0) { if (size_type(_M_end_of_storage._M_data - _M_finish) >= __n) {
//   _Tp __x_copy = __x; const size_type __elems_after = _M_finish - __position; pointer __old_finish = _M_finish;
//   if (__elems_after > __n) {...} else {...} } else _M_insert_overflow(__position, __x, _IsPODType(), __n); }`
//   -- __x_copy the only frame local, __elems_after homed in the dead `__x` slot, the tag temporaries in the
//   `__position` slot's top byte ([ebp+0Bh]), the two `fill` calls tail-merged. The STLport helpers need same-TU
//   noinline bodies (as 0x00579A4B) for that homing.
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

class CTCInventoryExperience {
public:
    class ExperienceSpendInfo {
    public:
        long currentSpend;                          // +0x0
        long initialSpend;                          // +0x4
        long levelUpSpend;                          // +0x8
    };
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                               // 0x005790C3
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                               // 0x00578C8D
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                               // 0x00578CF7
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                               // 0x00578C0F
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                              // 0x00579A4B
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
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

template void vector<CTCInventoryExperience::ExperienceSpendInfo>::_M_fill_insert(
    CTCInventoryExperience::ExperienceSpendInfo*, unsigned int, const CTCInventoryExperience::ExperienceSpendInfo&);

}