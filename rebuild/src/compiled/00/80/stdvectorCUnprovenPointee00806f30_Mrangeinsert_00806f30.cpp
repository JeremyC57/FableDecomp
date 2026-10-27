// std::vector<CUnprovenPointee_00806f30*>::_M_range_insert(iterator pos, CUnprovenPointee_00806f30* const* first, CUnprovenPointee_00806f30* const* last, const forward_iterator_tag&) -- STLport insert of a POD range: memmove the tail inside the spare capacity, else reallocate to size + max(size, n) and memmove prefix, range and suffix into the new block
// Leaf: ??$_M_range_insert@PBQAVCUnprovenPointee_00806f30@@@?$vector@PAVCUnprovenPointee_00806f30@@V?$allocator@PAVCUnprovenPointee_00806f30@@@std@@@std@@QAEXPAPAVCUnprovenPointee_00806f30@@PBQAV2@1ABUforward_iterator_tag@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00806f30, 496 bytes, thiscall, `ret 10h` (pos, first, last, the tag reference).
// Identity (descriptive): its callers are inline insert(pos, first, last) wrappers in the CTCTargetingPlayer targeting code (0x00807A80 by tail jump, 0x008087D0, and a call at 0x00803BB0); its own bytes are the 4-byte POD range insert (`sar 2`, memmove copies); the element type is unproven (a pointer is assumed).
// Callees: memmove 0x00BFEAE6 (every __true_type copy), malloc 0x00BFEA0E, free 0x00BFEA14.
// Shape (family L068): the landed vector<unsigned char>::_M_range_insert 0x009B3630 verbatim with a 4-byte element.
class CUnprovenPointee_00806f30;                       // descriptive: the pointee is unproven
extern "C" void* __cdecl malloc(unsigned int size);                                // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __true_type&)
{
    return (_ForwardIter)__copy_trivial(__first, __last, __result);
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator, _ForwardIterator) { }

template <class _RandomAccessIterator, class _Distance>
inline void distance(const _RandomAccessIterator& __first, const _RandomAccessIterator& __last, _Distance& __n)
{
    __n += __last - __first;
}

template <class _RandomAccessIterator, class _Distance>
inline void advance(_RandomAccessIterator& __i, _Distance __n)
{
    __i += __n;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __true_type _IsPODType;
    typedef __true_type _TrivialAss;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }

    template <class _ForwardIterator>
    void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last,
                         const forward_iterator_tag&)
    {
        if (__first != __last) {
            size_type __n = 0;
            distance(__first, __last, __n);

            if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
                const size_type __elems_after = this->_M_finish - __position;
                pointer __old_finish = this->_M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                    copy(__first, __last, __position);
                } else {
                    _ForwardIterator __mid = __first;
                    advance(__mid, __elems_after);
                    __uninitialized_copy(__mid, __last, this->_M_finish, _IsPODType());
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __elems_after;
                    copy(__first, __mid, __position);
                }
            } else {
                const size_type __old_size = size();
                const size_type __len = __old_size + (max)(__old_size, __n);
                pointer __new_start = this->_M_end_of_storage.allocate(__len);
                pointer __new_finish = __new_start;
                __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, _IsPODType());
                __new_finish = __uninitialized_copy(__first, __last, __new_finish, _IsPODType());
                __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, _IsPODType());
                _Destroy(this->_M_start, this->_M_finish);
                this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
                this->_M_start = __new_start;
                this->_M_finish = __new_finish;
                this->_M_end_of_storage._M_data = __new_start + __len;
            }
        }
    }
};

}
template void std::vector<CUnprovenPointee_00806f30* >::_M_range_insert<CUnprovenPointee_00806f30* const*>(CUnprovenPointee_00806f30**, CUnprovenPointee_00806f30* const*, CUnprovenPointee_00806f30* const*, const std::forward_iterator_tag&);