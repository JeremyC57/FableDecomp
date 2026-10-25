// _STL::__find_if(const char* first, const char* last, _Eq_char_bound<char_traits<char> > pred, const random_access_iterator_tag&) -- STLport four-way unrolled find_if over a character range: the first character equal to the bound one, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00841640, length 76 bytes, `ret 8`.
// Callers: the find_if wrapper 0x008416C0 (0x008416CA; it forwards the 1-byte predicate by value and homes the tag on the
//   predicate's slot) and seven sites 0x0084176C .. 0x00841A49; in the unrolled loop at 0x00841970 each call builds
//   `_Eq_char_bound(c)` from the next character (`mov [esp+24h],al ; push dword [esp+24h]`) and tests the result against
//   the range end -- the shape of STLport's inline `_Not_within_traits` predicate (find_first_not_of / find_last_not_of).
// Callees: none.
// Label: catalog "std::find<char*,char>" is a near miss: the value travels by value in the argument slot (`mov bl,
//   [esp+8]`), which is STLport's by-value `_Eq_char_bound<_Traits>` predicate of `find_if`, not find's `const _Tp&`.
// Shape: STLport 4.6 `_algobase.c` random-access `__find_if` verbatim (as 0x00695E30 of this chunk) in a /Gr algorithm
//   TU (`__fastcall`: first in ecx, last in edx, the predicate and the tag reference on the stack); the predicate's
//   char rides in bl, `_Traits::eq` is `cmp byte ptr [eax],bl`.
namespace _STL {
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
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred, const random_access_iterator_tag&)
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
}

typedef _STL::_Eq_char_bound<_STL::char_traits<char> > TEqCharBound;
template const char* __fastcall _STL::__find_if<const char*, TEqCharBound>(const char*, const char*, TEqCharBound, const _STL::random_access_iterator_tag&);