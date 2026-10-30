// std::vector<KnickKnack>::_M_fill_insert(KnickKnack* position, unsigned int n, const KnickKnack& x) -- STLport vector insert(pos, n, x) for the spot-the-addition knick-knacks: shift the tail inside the spare capacity and fill the gap from a local copy, else regrow through _M_insert_overflow
// Grade: DIFFER(316v316) -- position/elems_after take ebx/ebp swapped vs retail (ebp=position, ebx=elems_after) and the first branch spills old_finish-n into the dead position slot instead of the dead x slot; everything else (inlined placement-new fill loop, tag homing, calls) matches; this sweep: declared-only helpers -- inert
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D9D70, 320 bytes, thiscall `ret 0Ch`.
// Identity: reached from vector<KnickKnack>::resize 0x007D9FC0 (landed, it names this address as its _M_fill_insert)
//   and the insert(pos, n, x) front ends 0x007D9F60 / 0x007DA000. The catalog's "vector_Insert_16byte_Fill" is
//   descriptive.
// Callees: __uninitialized_copy(first, last, result, const __false_type&) 0x007D98C0 (`ret 8`), __copy_backward(first,
//   last, result, const random_access_iterator_tag&, int*) 0x007D9790 (`ret 0Ch`), fill(first, last, const T&)
//   0x007D96F0 (`ret 4`), _M_insert_overflow(pos, x, const __false_type&, n, atend) 0x007D9C90.
// Layout: Ego_r.pdb KnickKnack (16 bytes): Pos C3DVector +0, Thing CThingObject* +0xC, an empty user default
//   constructor (so __type_traits gives __false_type), the implicit copy.
// Shape (needs C3DVector's user copy constructor for the inline fill loop to keep the cursor in ecx): STLport 4.6 `_M_fill_insert` verbatim; the 16-byte x_copy lives in the frame, uninitialized_fill_n inlines as
//   the placement-new loop (count homed on the dead x slot), __copy_backward_ptrs inlines into the tagged
//   __copy_backward call.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CThingObject;

struct KnickKnack {                                                 // 16 bytes
    C3DVector     Pos;                                              // +0x00
    CThingObject* Thing;                                            // +0x0C
    KnickKnack() { }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                                   // 0x007D98C0
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                               _BidirectionalIter2 __result, const random_access_iterator_tag&,
                                               _Distance*);                                          // 0x007D9790
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                  // 0x007D96F0

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp> class allocator { public: allocator() { } };
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
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                                   // 0x007D9C90
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

template void vector<KnickKnack>::_M_fill_insert(KnickKnack*, unsigned int, const KnickKnack&);
}
