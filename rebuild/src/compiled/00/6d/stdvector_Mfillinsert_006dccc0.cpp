// std::vector::_M_fill_insert(CAttackHistoryCombo* position, unsigned int n, const CAttackHistoryCombo& x) -- STLport insert of n copies of a hero attack-history combo: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DCCC0, 266 bytes (+ int3 padding to 272), thiscall, `ret 0Ch`.
// Identity: the catalog's "PairVector_Resize" is a descriptive auto label. The element is CAttackHistoryCombo
//   (Ego_r.pdb: vector<pair<EAttackHistoryType, EDamageAttribute> > AttackCombo; float Multiplier; 16 bytes with the
//   12-byte STLport vector), the vector<CAttackHistoryCombo> CHeroExperienceDef::AttackHistoryCombos; every callee is a
//   landed row of the same vector: _M_insert_overflow 0x006DCA50 (`push 0` = atend false) and the /Gr helpers below.
// Callees: the pair-vector copy constructor 0x004F9408 (x_copy's AttackCombo; the implicit combo copy is inline),
//   __uninitialized_copy 0x006DBCE0 (twice), __copy_backward(.., const random_access_iterator_tag&, int*) 0x006DBDC0,
//   fill 0x006DBD20, uninitialized_fill_n 0x006DC670, free 0x00BFEA14 (x_copy's inline vector destructor).
// Shape: STLport 4.6 `_M_fill_insert` verbatim (the 0x006B6B30 / 0x00618860 model): `_Tp __x_copy = __x;` in a 16-byte
//   frame local (out-of-line vector copy + the Multiplier word), the slide or the short-tail path, both fill calls
//   tail-merged before x_copy's inline destructor (`if (_M_start) free(_M_start)`). The helpers are same-TU noinline
//   bodies, so VC7.1 homes the empty tags (and the spilled `old_finish - n`) in the dead position argument slot.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (allocator, not emitted here)
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

enum EAttackHistoryType { ATTACK_HISTORY_TYPE_0 };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_0 };

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    vector(const vector<_Tp, _Alloc>& __x);                 // 0x004F9408 for the attack-combo pairs (landed)
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);         // 0x006DCA50 for CAttackHistoryCombo (landed)
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

} // namespace std

class CAttackHistoryCombo {                                 // 16 bytes in retail
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                            // +0x0C
};

namespace std {

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU stand-ins for the landed helpers, kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                  // 0x006DBCE0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                  // 0x006DBDC0
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                  // 0x006DBD20
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                  // 0x006DC670
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

template void vector<CAttackHistoryCombo>::_M_fill_insert(CAttackHistoryCombo*, unsigned int, const CAttackHistoryCombo&);

} // namespace std