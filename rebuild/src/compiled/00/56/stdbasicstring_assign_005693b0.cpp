// std::basic_string::assign(const char* f, const char* l) -- STLport range assign: overwrite in place and erase the tail when the range fits, else overwrite size() bytes and append the rest
// Leaf: ?assign@?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBD0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005693B0, 151 bytes, thiscall `ret 8`, returns *this. Callers include the
//   transaction constructor 0x00569C50 (three `= ""` assignments: push lit ; push lit). The catalog label
//   "Vector_Insert" is descriptive.
// Callees: memmove @ 0x00BFEAE6 (char_traits::move inside erase), basic_string<char>::append<const char*>(const char*,
//   const char*, const forward_iterator_tag&) @ 0x00567D60 (the `<const char*>` instance of the STLport member template;
//   the byte-identical 0x00567400 is the `<char*>` instance, not a second TU copy: retail has no ICF and one symbol's
//   COMDAT copies fold).
// Instance: the STLport 4.6 class template as written (`basic_string<_CharT,_Traits,_Alloc>` over `_String_base`,
//   `char_traits<char>` a class): `assign(const _CharT*, const _CharT*)` is the non-template member, and its
//   `append(__f + size(), __l)` deduces `const char*`, binding `??$append@PBD@...` 0x00567D60.
// Layout: STLport basic_string<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x `assign(const _CharT*, const _CharT*)` verbatim: `n = l - f; if ((size_type)n <= size())
//   { _Traits::copy(_M_start, f, n); erase(begin() + n, end()); } else { _Traits::copy(_M_start, f, size());
//   append(f + size(), l); }`. The traits copy/move are `n == 0 ? s1 : memcpy/memmove(..)` (the memcpy is the
//   /Oi `rep movsd ; rep movsb` intrinsic, the zero tests stay); erase moves the terminator with
//   `(finish - last) + 1` (not folded); append dispatches to the forward-iterator overload, whose tag
//   temporary takes the dead `l` argument slot.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

namespace std {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _CharT> class char_traits;
template <> class char_traits<char> {
public:
    static char* move(char* __s1, const char* __s2, unsigned int __n)
    {
        return (__n == 0 ? __s1 : (char*)memmove(__s1, __s2, __n * sizeof(char)));
    }
    static char* copy(char* __s1, const char* __s2, unsigned int __n)
    {
        return (__n == 0 ? __s1 : (char*)memcpy(__s1, __s2, __n * sizeof(char)));
    }
};

template <class _Tp> class allocator { };

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
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : public _String_base<_CharT, _Alloc> {
public:
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    typedef unsigned int size_type;
    typedef _CharT* iterator;

    size_type size() const { return this->_M_finish - this->_M_start; }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }

    iterator erase(iterator __first, iterator __last)
    {
        if (__first != __last) {
            // The move includes the terminating _CharT().
            _Traits::move(__first, __last, (this->_M_finish - __last) + 1);
            _CharT* __new_finish = this->_M_finish - (__last - __first);
            this->_M_finish = __new_finish;
        }
        return __first;
    }

    template <class _ForwardIter>
    _Self& append(_ForwardIter __first, _ForwardIter __last, const forward_iterator_tag&);   // <const char*> 0x00567D60
    template <class _InputIter> _Self& append(_InputIter __first, _InputIter __last)
    {
        return append(__first, __last, forward_iterator_tag());
    }

    _Self& assign(const _CharT* __f, const _CharT* __l);
};

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::assign(const _CharT* __f, const _CharT* __l)
{
    int __n = __l - __f;
    if ((size_type)__n <= size()) {
        _Traits::copy(this->_M_start, __f, __n);
        erase(begin() + __n, end());
    }
    else {
        _Traits::copy(this->_M_start, __f, size());
        append(__f + size(), __l);
    }
    return *this;
}

template basic_string<char>& basic_string<char>::assign(const char*, const char*);

}
