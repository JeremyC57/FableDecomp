// std::__find_if(const char* first, const char* last, _Not_within_traits<char_traits<char> > pred, const random_access_iterator_tag&) -- STLport four-way unrolled find_if over a character range: the first character NOT in pred's set [pred._M_first, pred._M_last), else last (the string find_first_not_of scan)
// Leaf: ??$__find_if@PBDU?$_Not_within_traits@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00841900, 368 bytes, /Gr `__fastcall` (first in ecx, last in edx), `ret 0Ch`: the
//   8-byte predicate (set first, set last) by value plus the empty tag reference.
// Identity: callers 0x0083F789 (0x0083F650), 0x00841ABF, 0x00841B4A, 0x00841DB7 -- the LHNetworkLib HTTP string scans
//   (basic_string::find_first_not_of / find_last_not_of over a delimiter set). The predicate's inner find is the
//   _Eq_char_bound __find_if 0x00841640 (agent459 chunk). The catalog's "String_FindFirstOf" is descriptive.
// Callees: __find_if(const char*, const char*, _Eq_char_bound<char_traits<char> >, const random_access_iterator_tag&)
//   0x00841640 (`ret 8`).
// Shape: STLport 4.6 `__find_if` verbatim with `_Not_within_traits::operator()` inline (`find_if(_M_first, _M_last,
//   _Eq_char_bound(c)) == _M_last`): VC7.1 inlines the inner __find_if only at the first predicate call of the
//   unrolled loop (its trip count hoisted to a frame word) and calls it out of line at the other six; the 1-byte
//   _Eq_char_bound temporary and the inner tag reference share the dead set-first argument slot.
namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _CharT> class char_traits;
template <> class char_traits<char> {
public:
    typedef char char_type;
    static bool eq(const char& __c1, const char& __c2) { return __c1 == __c2; }
};

template <class _Traits> struct _Eq_char_bound {
    typedef typename _Traits::char_type char_type;
    char_type __val;
    _Eq_char_bound(char_type __c) : __val(__c) { }
    bool operator()(const char_type& __c) const { return _Traits::eq(__c, __val); }
};

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}

template <class _Traits> struct _Not_within_traits {
    typedef const typename _Traits::char_type* _Pointer;
    const _Pointer _M_first;
    const _Pointer _M_last;

    _Not_within_traits(_Pointer __f, _Pointer __l) : _M_first(__f), _M_last(__l) { }

    bool operator()(const typename _Traits::char_type& __x) const
    {
        return find_if((_Pointer)_M_first, (_Pointer)_M_last, _Eq_char_bound<_Traits>(__x)) == (_Pointer)_M_last;
    }
};
}

typedef std::_Not_within_traits<std::char_traits<char> > TNotWithin;
template const char* __fastcall std::__find_if<const char*, TNotWithin>(const char*, const char*, TNotWithin,
                                                                        const std::random_access_iterator_tag&);