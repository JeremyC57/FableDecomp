// std::vector::_M_fill_insert(CGameMapCell* position, unsigned int n, const CGameMapCell& x) -- STLport insert of n copies of a 7-byte game-map cell: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8050, 336 bytes (+ int3 padding), thiscall, `ret 0Ch`, /GS cookie.
// Identity: the catalog's "Vector_InsertAndFill" is a descriptive auto label. The element is CGameMapCell (Ego_r.pdb:
//   AtmosThemes uchar[3], AtmosBlends uchar[2], SoundTheme schar, Music uchar; 7 bytes, alignment 1) of CGameMap's
//   vector<CGameMapCell>; callers: the insert thunk 0x006B81E0 (jmp) and resize 0x006B8241.
// Callees: __uninitialized_copy(first, last, result, const __false_type&) 0x006B7BB0 (twice), __copy_backward(.., const
//   random_access_iterator_tag&, int*) 0x006B7A00, fill 0x006B78E0, uninitialized_fill_n 0x006B7CC0 (all landed
//   `__fastcall` rows of the /Gr TU), _M_insert_overflow 0x006B7F70 (thiscall, atend defaulted false: `push 0`),
//   __security_check_cookie 0x00BFE9F9 (/GS: the char-array x_copy local is a protected buffer; cookie 0x0139C8A8).
// Shape: STLport 4.6 `_M_fill_insert` verbatim (the 0x006B6B30 model): `_Tp __x_copy = __x;` is the inline 7-byte
//   block copy (dword, word, byte), the helpers are same-TU noinline bodies, so VC7.1 homes the empty tags and the spilled
//   `old_finish - n` / `n - elems_after` words in the dead x argument slot; both fill calls tail-merge. /GS (the
//   `// Flags:` line) emits the cookie store at entry and the `__security_check_cookie` call on both exits.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (allocator, not emitted here)
inline void* operator new(unsigned int, void* p) { return p; }

struct CGameMapCell {                                       // 7 bytes, alignment 1
    unsigned char AtmosThemes[3];                           // +0
    unsigned char AtmosBlends[2];                           // +3
    signed char   SoundTheme;                               // +5
    unsigned char Music;                                    // +6
    CGameMapCell() { }
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x006B7F70
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
{                                                                                  // 0x006B7BB0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                  // 0x006B7A00
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                  // 0x006B78E0
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                  // 0x006B7CC0
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

template void vector<CGameMapCell>::_M_fill_insert(CGameMapCell*, unsigned int, const CGameMapCell&);

} // namespace std