// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CUnprovenElement_005269b0>) STLport growth insert for a 16-byte trivially copyable element: allocate size + max(size, fill_len) elements, copy the head out of line, construct x once (or fill out of line fill_len times), copy the tail unless atend, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052B030, 207 bytes, thiscall, `ret 14h` (five arguments).
// Identity (probable): STLport 4.6 vector<T>::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend) -- the
//   shape (max(size, fill_len) growth, malloc 0x00BFEA0E, head copy / x construct / fill / tail copy, free 0x00BFEA14,
//   _M_set) is the landed 0x0084F5E0 one; its only caller is the unreferenced push_back stub at 0x0052B970 (call at
//   0x0052B9AE), so the 16-byte element type is unproven: it reuses the descriptive element of the landed callee
//   0x005269B0 (`std::__uninitialized_copy<CUnprovenElement_005269b0*, ...>`). The landed fill_n callee 0x005269F0 was
//   named over a second descriptive spelling (struct CUnprovenElement16_005269f0) for the SAME element; it is called
//   through that bound spelling (pointer casts, no code) so each address keeps its one name.
// Shape: the 207-byte form is the implicit (single-register) element copy in the fill_len == 1 arm (the landed
//   0x0084F5E0 needed C3DVector's memberwise user copy ctor for 209); the helpers' bodies sit in the TU (noinline,
//   __fastcall) so the const __false_type& tag is homed in the live `atend` slot.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CUnprovenElement_005269b0 {                          // 16 bytes (descriptive: element type not proven)
public:
    unsigned long Word0;
    unsigned long Word1;
    unsigned long Word2;
    unsigned long Word3;
};
struct CUnprovenElement16_005269f0 {                       // the same element under 0x005269F0's bound spelling
    unsigned long Word0;
    unsigned long Word1;
    unsigned long Word2;
    unsigned long Word3;
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                          // 0x005269B0 (same-TU body)
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                          // 0x005269F0 (same-TU body)
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = (pointer)__uninitialized_fill_n((CUnprovenElement16_005269f0*)__new_finish, __fill_len,
                                                       (const CUnprovenElement16_005269f0&)__x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
typedef CUnprovenElement_005269b0 _Elem;
template void vector<_Elem>::_M_insert_overflow(_Elem*, const _Elem&, const __false_type&, unsigned int, bool);
} // namespace std