// _STL::basic_string::_M_assign_dispatch(char* f, char* l, const __false_type&) -- STLport string assign from a char* range: overwrite the existing characters in place, then erase the leftover tail or append the rest of the range
// Leaf: ??$_M_assign_dispatch@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569140, length 104 bytes, `ret 0Ch` (thiscall: f, l, the tag reference).
// Callers: 20 sites (0x00561041, 0x005697AF, 0x00569999, LHNetworkLib 0x0083F520.., 0x008F8D8C, 0x0090AFFB): the
//   `assign(first, last)` front end with char* iterators (`_M_assign_dispatch(f, l, _Integral())`).
// Callees: memmove @ 0x00BFEAE6 (char_traits::move inside the inline erase), basic_string::append<char*>(first, last,
//   const forward_iterator_tag&) @ 0x00567400 (landed; out of line here).
// Label: catalog "std::string_InsertRange" is descriptive; the body is STLport 4.x `_M_assign_dispatch`.
// Shape: STLport `_string.h` verbatim: `pointer __cur = _M_start; while (__f != __l && __cur != _M_finish) {
//   _Traits::assign(*__cur, *__f); ++__f; ++__cur; } if (__f == __l) erase(__cur, end()); else append(__f, __l);
//   return *this;` -- erase is inline (`traits::move(first, last, (_M_finish - last) + 1)` with its zero-length test,
//   then `_M_finish -= last - first`; the char _Destroy is empty) and append's forward_iterator_tag temporary is homed in
//   the dead `f` argument slot (`lea edx,[esp+0Ch]`).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace _STL {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };

template <class _CharT> class allocator { };

template <class _CharT, class _IntT> class __char_traits_base {
public:
    typedef _CharT char_type;
    static void assign(char_type& __c1, const char_type& __c2) { __c1 = __c2; }
    static char_type* move(char_type* __s1, const char_type* __s2, unsigned int __n)
    {
        return (__n == 0 ? __s1 : (char_type*)memmove(__s1, __s2, __n * sizeof(char_type)));
    }
};
template <class _CharT> class char_traits;
template <> class char_traits<char> : public __char_traits_base<char, int> { };

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _Tp* _M_end_of_storage;                          // +0x08 (_STLP_alloc_proxy over the empty allocator)
};

template <class _CharT, class _Traits, class _Alloc> class basic_string : public _String_base<_CharT, _Alloc> {
public:
    typedef _CharT* pointer;
    typedef _CharT* iterator;
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        if (__first != __last) {
            // The move includes the terminating _CharT().
            _Traits::move(__first, __last, (this->_M_finish - __last) + 1);
            pointer __new_finish = this->_M_finish - (__last - __first);
            _Destroy(__new_finish + 1, this->_M_finish + 1);
            this->_M_finish = __new_finish;
        }
        return __first;
    }
    template <class _ForwardIter>
    _Self& append(_ForwardIter __first, _ForwardIter __last, const forward_iterator_tag&);   // <char*> 0x00567400
    template <class _InputIter> _Self& append(_InputIter __first, _InputIter __last)
    {
        return append(__first, __last, forward_iterator_tag());
    }
    template <class _InputIter> _Self& _M_assign_dispatch(_InputIter __f, _InputIter __l, const __false_type&)
    {
        pointer __cur = this->_M_start;
        while (__f != __l && __cur != this->_M_finish) {
            _Traits::assign(*__cur, *__f);
            ++__f;
            ++__cur;
        }
        if (__f == __l)
            erase(__cur, end());
        else
            append(__f, __l);
        return *this;
    }
};
}

typedef _STL::basic_string<char, _STL::char_traits<char>, _STL::allocator<char> > TString;
template TString& TString::_M_assign_dispatch<char*>(char*, char*, const _STL::__false_type&);