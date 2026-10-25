// std::vector::_M_fill_insert(CTCInventoryMap::CUIMiniMapMarker* position, unsigned int n, const CUIMiniMapMarker& x) -- STLport insert of n copies of a mini-map marker: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00618860, 304 bytes (302 without padding), thiscall `ret 0Ch`. Callers: the
//   insert(pos, n, x) jmp thunk 0x00618F60 and resize(n, x) (`call` at 0x006190AD) of CTCInventoryMap::MiniMapMarkers.
// Callees: CUIMiniMapMarker copy constructor 0x00612980 (landed; x_copy), __uninitialized_copy(.., const __false_type&)
//   0x00613DD0, __copy_backward(.., const random_access_iterator_tag&, int*) 0x00613390, fill 0x00613180,
//   __uninitialized_fill_n(.., const __false_type&) 0x00613E30 (all `__fastcall`, COMDAT-folded with other 28-byte
//   element types, hence their propagated labels), _M_insert_overflow 0x006182F0 (atend defaulted false: `push 0`),
//   ~CCharString 0x0099EAE0 (x_copy's inline destructor).
// Label correction: the catalog "Vector_InsertRange_MiniMapMarker" -- this is STLport 4.6 `_M_fill_insert`.
// Model: PDB CTCInventoryMap::CUIMiniMapMarker (28 bytes): Region, UIDefName (CCharString), Position (C2DVector),
//   DefNameLength, GraphicIndex, Angle; the copy constructor is the out-of-line 0x00612980, the destructor implicit.
// Shape: STLport 4.6 `_M_fill_insert` verbatim; the helpers are same-TU noinline bodies (auto_inline off) so VC7.1 homes
//   the empty tags and `old_finish - n` / `n - elems_after` in the dead n argument slot as retail does; the two
//   `fill(.., x_copy)` calls tail-merge into one block before x_copy's destructor.
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                                       // 0x0099EC30
    ~CCharString();                                                              // 0x0099EAE0
    CCharString& operator=(const CCharString& other);
};

struct C2DVector {
    float X, Y;
};

class CTCInventoryMap {
public:
    class CUIMiniMapMarker {
    public:
        long        Region;                                                      // +0x00
        CCharString UIDefName;                                                   // +0x04
        C2DVector   Position;                                                    // +0x08
        long        DefNameLength;                                               // +0x10
        long        GraphicIndex;                                                // +0x14
        float       Angle;                                                       // +0x18
        CUIMiniMapMarker(const CUIMiniMapMarker& other);                         // 0x00612980
    };
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                                                 // 0x00613DD0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                                                 // 0x00613E30
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x00613390
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x00613180
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp>
__forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
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
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x006182F0
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

template void vector<CTCInventoryMap::CUIMiniMapMarker>::_M_fill_insert(CTCInventoryMap::CUIMiniMapMarker*, unsigned int, const CTCInventoryMap::CUIMiniMapMarker&);
}