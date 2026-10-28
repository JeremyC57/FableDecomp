// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<std::pair<CCharString, long> >) STLport insert(pos, n, x): with room, shift the tail in place (uninitialized copy, copy_backward, fill from a copy of x), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@U?$pair@VCCharString@@J@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4850, 278 bytes, thiscall, `ret 0Ch`.
// Identity (proven): the 8-byte element is copied as an out-of-line CCharString copy (0x0099EC30) plus one dword and
//   destroyed through ~CCharString (0x0099EAE0); its helpers are this TU's pair<CCharString, long> rows (fill 0x006D3AF0,
//   _M_insert_overflow 0x006D4750, both catalogued over pair<CCharString,long>), and its callers are the
//   CPersistContext_TransferableVector<std::pair<CCharString,long> >::Resize 0x006D4AA0 (0x006D4B1B) and the insert/resize
//   wrappers 0x006D49C0 (jmp) / 0x006D4A08 / 0x006D4A7E. Catalog label "_Insert_range_fill_8byte" is behavioural.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (x_copy), __uninitialized_copy 0x006D3770,
//   __copy_backward<pair*, pair*, int> 0x006D3610 (the tag and the null Distance* on the stack; __copy_backward_ptrs is
//   inline), fill 0x006D3AF0, uninitialized_fill_n 0x006D3B20 (tag-less wrapper), _M_insert_overflow 0x006D4750,
//   ~CCharString 0x0099EAE0 (x_copy).
// Model: the landed twin 0x0049B5A0 (vector<CTexture>::_M_fill_insert), STLport 4.6 verbatim; here __copy_backward is the
//   out-of-line helper and __copy_backward_ptrs inlines (retail passes the random_access tag and the 0 Distance*).
//   _M_insert_overflow is declared public to keep the bound spelling of 0x006D4750.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{                                                                                 // 0x006D3770
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x006D3B20
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x006D3610
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x006D3AF0
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

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
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                              // 0x006D4750
protected:
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<pair<CCharString, long> >::_M_fill_insert(pair<CCharString, long>*, unsigned int,
                                                               const pair<CCharString, long>&);
}
