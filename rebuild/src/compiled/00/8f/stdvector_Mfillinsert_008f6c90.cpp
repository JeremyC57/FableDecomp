// std::vector::_M_fill_insert(CHeightMapCell* position, unsigned int n, const CHeightMapCell& x) -- STLport insert of n copies of a 12-byte height-map cell: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F6C90, 336 bytes (+ int3 padding), thiscall, `ret 0Ch`, /GS cookie.
// Identity: the catalog's "std_deque_insert_range<CSoundPair>" is propagated across byte-identical bodies. The only
//   caller is vector<CHeightMapCell>::resize(n, const T&) 0x008F6E30 (call at 0x008F6E7D, `insert(end(), n - size(), x)`),
//   which the landed CHeightMap constructor 0x008F63C0 calls on its Cells member: the element is CHeightMapCell
//   (Ego_r.pdb: float Height; uchar EngineThemePaletteIndex[3]; uchar EngineBlend[2]; five 1-bit flags; 12 bytes).
// Callees (the /Gr TU's `__fastcall` STLport helpers, all landed): __uninitialized_copy(first, last, result,
//   const __false_type&) 0x008F6800 (twice), __copy_backward(.., const random_access_iterator_tag&, int*) 0x008F6680,
//   fill 0x008F6580, uninitialized_fill_n 0x008F6910; _M_insert_overflow 0x008F6BB0 (thiscall, atend defaulted false:
//   `push 0`); __security_check_cookie 0x00BFE9F9 (/GS: the cell's char arrays make the x_copy local a protected
//   buffer; cookie 0x0139C8A8).
// Shape: STLport 4.6 `_M_fill_insert` verbatim (the landed-lane 0x006B8050 model): `_Tp __x_copy = __x;` is the inline
//   12-byte copy, the helpers are same-TU noinline bodies so VC7.1 homes the empty tags and the spilled
//   `old_finish - n` / `n - elems_after` words in the dead x argument slot; both fill calls tail-merge.
//   The cell model is local (as the landed CHeightMap constructor's): the generated engine header is pack(1) and has
//   no constructor.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (allocator, not emitted here)
inline void* operator new(unsigned int, void* p) { return p; }

class CHeightMapCell { public:                                     // 12 bytes
    float         Height;                                   // +0x0
    unsigned char EngineThemePaletteIndex[3];               // +0x4
    unsigned char EngineBlend[2];                           // +0x7
    unsigned char Changed : 1;                              // +0x9
    unsigned char Passable : 1;
    unsigned char ShorePoint : 1;
    unsigned char CameraPassable : 1;
    unsigned char NavPreferred : 1;
    CHeightMapCell() { }
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x008F6BB0
    public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

} // namespace std

namespace std {

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU stand-ins for the landed helpers, kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                  // 0x008F6800
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                  // 0x008F6680
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                  // 0x008F6580
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                  // 0x008F6910
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

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

template void vector<CHeightMapCell>::_M_fill_insert(CHeightMapCell*, unsigned int, const CHeightMapCell&);

} // namespace std