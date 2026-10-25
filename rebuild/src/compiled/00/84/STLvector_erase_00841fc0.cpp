// std::vector::erase(iterator first, iterator last) -- STLport range erase of a vector<basic_string<char> >: assign the tail down over the erased range, destroy (free) the strings left past the new end, return first
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00841FC0, length 84 bytes (+ int3 padding), thiscall, `ret 8`.
// Callers: 9 sites, among them LHNetworkLib::LHHttpHeaders::Reset 0x0083FBC0 (landed; clear() as erase(begin(), end()))
//   and LHHttpHeaderStatus::Reset 0x0083FB60, 0x0083FFCF / 0x0083FFF2 (LHHttp2::Reset), 0x00840674 .. 0x00842027.
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) @ 0x00841DE0 (landed; __fastcall, the
//   string assignment loop), free @ 0x00BFEA14 (each dead string's inline destructor).
// Label: catalog "std::vector_move_and_cleanup" is descriptive.
// Shape: STLport 4.6 `erase(iterator __first, iterator __last) { pointer __i = __copy_ptrs(__last, _M_finish, __first,
//   _TrivialAss()); _Destroy(__i, _M_finish); _M_finish = __i; return __first; }`: __copy_ptrs (the __false_type arm)
//   inlines to the counted __copy call with `random_access_iterator_tag()` and `(int*)0`; retail homes that tag on the
//   LIVE `first` slot (returned at the end), which takes a same-TU stand-in body of __copy that never reads the tag;
//   _Destroy's loop and the string destructor (`if (_M_start) free(_M_start)`) are inline.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _CharT> class char_traits { };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT, class _Traits, class _Alloc> class basic_string : public _String_base<_CharT, _Alloc> {
public:
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    ~basic_string() { }
    _Self& _M_assign_dispatch(const _CharT* __f, const _CharT* __l, const __false_type&);   // 0x00569140 (this chunk)
    _Self& operator=(const _Self& __s)
    {
        if (&__s != this)
            _M_assign_dispatch(__s._M_start, __s._M_finish, __false_type());
        return *this;
    }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

// Same-TU stand-in body of the out-of-line counted copy (the STLport text of 0x00841DE0): it never reads the tag, so
// VC7.1 may home the tag temporary on the live `first` slot, as retail.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > TString;
template TString* std::vector<TString>::erase(TString*, TString*);