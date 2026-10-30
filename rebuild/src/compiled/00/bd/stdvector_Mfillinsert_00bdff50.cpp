// std::vector::_M_fill_insert(C3DBoundingSphere* position, unsigned int n, const C3DBoundingSphere& x) -- (vector<C3DBoundingSphere>) STLport insert of n copies of a 16-byte bounding sphere: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VC3DBoundingSphere@@V?$allocator@VC3DBoundingSphere@@@std@@@std@@QAEXPAVC3DBoundingSphere@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDFF50, 316 bytes, thiscall `ret 0Ch`, no EH frame (STLport built without exceptions).
// Identity: the landed vector<C3DBoundingSphere>::resize 0x00BE0140 (the function-local static sphere_list of
//   CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren 0x00BDF100) calls this row through the inline
//   `insert(pos, n, x)` and binds this exact `_M_fill_insert` spelling; the landed _M_insert_overflow 0x00BDFE70 and
//   __uninitialized_copy 0x00BDF880 (bound `??$__uninitialized_copy@PAVC3DBoundingSphere@@...`) are this row's callees.
//   The `sar 4` stride is the 16-byte Ego_r.pdb C3DBoundingSphere (class key V). STLport 4.6 vector.c verbatim.
// Callees (free helpers __fastcall): __uninitialized_copy 0x00BDF880 (twice), __copy_backward<..., int> 0x00BDF600
//   (five arguments: behind the inline __copy_backward_ptrs), fill 0x00BDF580 (twice), _M_insert_overflow 0x00BDFE70;
//   uninitialized_fill_n is inline here (the placement-new null-tested four-dword loop).
// Shape: x_copy (16 bytes) is the whole frame; retail homes the `__false_type()` / iterator tags on the dead x slot (finish - n
//   and n - elems_after are stored there and re-read), so __uninitialized_copy and __copy_backward get same-TU noinline
//   bodies that never touch the tag (the live-slot rule of 0x00BCDCA0). The C3DBoundingSphere user copy ctor (PDB) is the
//   memberwise copy of the inline fill loop.
inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {                                          // Ego_r.pdb, 12 bytes
public:
    float X;
    float Y;
    float Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // PDB ??0C3DVector@@QAE@ABV0@@Z
};
class C3DBoundingSphere {                                  // Ego_r.pdb, 16 bytes
public:
    C3DVector Center;                                      // +0x0
    float     Radius;                                      // +0xC
    C3DBoundingSphere(const C3DBoundingSphere& other)      // PDB ??0C3DBoundingSphere@@QAE@ABV0@@Z
        : Center(other.Center), Radius(other.Radius) { }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

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
{                                                                                              // 0x00BDF880
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
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
{                                                                                              // 0x00BDF600
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
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00BDF580

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00BDFE70
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

template void std::vector<C3DBoundingSphere>::_M_fill_insert(C3DBoundingSphere*, unsigned int, const C3DBoundingSphere&);