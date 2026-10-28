// std::vector<C3DVector>::insert(iterator position, const C3DVector& x) -- STLport 4.6 single-element insert: construct at the end, or shift the tail up one and assign, or grow through _M_insert_overflow; returns begin() + the old index
// Leaf: ?insert@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEPAVC3DVector@@PAV3@ABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007653B0, 253 bytes, thiscall `ret 8`.
// Identity (proven): STLport 4.6 _vector.h `insert(iterator, const _Tp&)` line for line over the 12-byte element; the callees
//   are already bound for vector<C3DVector>: _M_insert_overflow(pointer, const C3DVector&, const __false_type&, size_type, bool)
//   @ 0x0066E370 and __copy_backward<C3DVector*, C3DVector*, int> @ 0x0075B5B0 (random-access tag, (int*)0). C3DVector's user
//   copy constructor makes _IsPODType / _TrivialAss __false_type. The catalog label "std::vector_InsertBefore" is descriptive.
// Shape: the copies are C3DVector's inline user copy (three dwords); x_copy is a frame local assigned back after the shift.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*);                                          // 0x0075B5B0
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
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
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                            size_type __fill_len, bool __atend = false);                           // 0x0066E370
public:
    iterator insert(iterator __position, const _Tp& __x)
    {
        size_type __n = __position - begin();
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            if (__position == end()) {
                _Construct(this->_M_finish, __x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish, *(this->_M_finish - 1));
                ++this->_M_finish;
                _Tp __x_copy = __x;
                __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, _TrivialAss());
                *__position = __x_copy;
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), 1UL);
        return begin() + __n;
    }
};
template class vector<C3DVector>;
}   // namespace std