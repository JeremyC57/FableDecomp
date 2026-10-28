// std::vector<std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > >::_M_fill_insert(iterator position, size_type n, const value_type& x) -- STLport 4.6 insert of n copies of x before position (in place when capacity allows, else through _M_insert_overflow)
// Leaf: ?_M_fill_insert@?$vector@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@V?$allocator@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@@2@@std@@QAEXPAU?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@2@IABU32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FC7A0, 319 bytes, thiscall `ret 0Ch`.
// Identity (proven): the body is STLport 4.6 _vector.c `_M_fill_insert` line for line (n == 0 test, capacity test, x_copy, the
//   elems_after > n split with __uninitialized_copy / __copy_backward / fill, else uninitialized_fill_n / __uninitialized_copy /
//   fill, else _M_insert_overflow(position, x, _IsPODType(), n) with atend = false) over the 12-byte element whose callees are
//   already landed for pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > (the keyed animation-event vector):
//   _M_insert_overflow 0x006FC6A0, CSmallVector::Copy 0x006FB5C0 (FableWin.pdb, protected), __uninitialized_copy 0x006FBAE0,
//   __uninitialized_fill_n 0x006FBB30, __copy_backward 0x006FB830, fill 0x006FBBD0. The catalog label "AnimEventVector_Insert"
//   is descriptive.
// Callees: as above, plus operator delete[] @ 0x00BFEB1C (x_copy's CSmallVector destructor).
// Shape: the tag-taking helpers are same-TU noinline __fastcall instantiations with their STLport bodies (declared-only
//   they spill `finish - n` into the x argument slot instead of retail's n slot); at landing the retail COMDATs win.
void __cdecl operator delete[](void* block);              // 0x00BFEB1C
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;
};
}
template <class T, int N> class CSmallVector {
public:
    T* Start;                                             // +0x00
    int Size;                                             // +0x04
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    ~CSmallVector() { Clear(); }
    const CSmallVector& operator=(const CSmallVector& other) { Copy(other); return *this; }   // PDB ??4 returns ABV0
protected:
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    void Copy(const CSmallVector& other);                 // 0x006FB5C0
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
    pair& operator=(const pair& __p) { first = __p.first; second = __p.second; return *this; }
};
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// Same-TU out-of-line instantiations (the STLport 4.6 bodies; retail 0x006FBAE0, 0x006FBB30, 0x006FB830, 0x006FBBD0).
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result, const random_access_iterator_tag&,
                                                                   _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
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
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                            size_type __fill_len, bool __atend = false);                           // 0x006FC6A0
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}
template void vector<pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > >::_M_fill_insert(
    pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >*, unsigned int,
    const pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >&);
}   // namespace std