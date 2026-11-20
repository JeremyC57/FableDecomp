// std::operator+(const char* s, const basic_string& y) -- STLport string concatenation of a C string and a string: reserve strlen(s) + size(), append both, return the result by value
// Leaf: ??$?HDV?$char_traits@D@std@@V?$allocator@D@1@@std@@YA?AV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@0@PBDABV10@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00841C50, 210 bytes, __cdecl returning the string through the hidden pointer (`ret`).
// Identity: STLport 4.x `operator+(const _CharT* __s, const basic_string<_CharT,_Traits,_Alloc>& __y)` -- the landed
//   operator+(const basic_string&, const basic_string&) 0x00569690 with the first operand a C string: `__n =
//   _Traits::length(__s)`, `__result.append(__s, __s + __n)` then `__result.append(__y)`. Sibling of 0x00841B70. The manifest
//   label "String_PrependFromRange" is descriptive.
// Callees: malloc 0x00BFEA0E, basic_string::append<const char*>(first, last, const forward_iterator_tag&) 0x00567D60 and
//   append<char*> 0x00567400 (landed; same-TU `__declspec(noinline)` stand-ins), _M_range_initialize 0x00567D00 (the return
//   copy), free 0x00BFEA14, puts 0x00BFEAF2 / abort 0x00BFEAEC (the inlined length error, ending `jmp abort`).
// Shape: STLport 4.6 _string.c operator+ on the shared stlport/fable_string.h basic_string<char> (THROW_INLINE: retail expands the
//   _Reserve_t constructor's length error in place, `push "basic_string" ; call puts ; jmp abort`); the private range append
//   (`append(f, l, const forward_iterator_tag&)`, same-TU body under auto_inline(off)) and _M_range_initialize stay out of line.
#define FABLE_RETAIL_STLP_STRING_THROW_INLINE
#include "stlport/fable_string.h"

namespace std {

// Fidelity: same-TU definition of 0x00567400 / 0x00567D60 (retail TU-mate): the STLport 4.6 _string.c template body verbatim as
//   stlport/fable_string_bodies.h has it; both instances stay out of line (auto_inline(off)) and bind the private AAE spellings.
// Same-TU body of the private STLport 4.6 range append (_string.c, as stlport/fable_string_bodies.h has it; that header cannot be
// included beside THROW_INLINE): retail's TU saw it, so VC7.1's mod/ref summary knows the call leaves __result's words alone.
#pragma auto_inline(off)
template <class _CharT, class _Traits, class _Alloc> template <class _ForwardIter>
basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::append(_ForwardIter __first, _ForwardIter __last,
                                                                                     const forward_iterator_tag&)
{
    if (__first != __last) {
        const size_type __old_size = size();
        difference_type __n = __last - __first;
        if (size_type(__n) > max_size() || __old_size > max_size() - size_type(__n)) this->_M_throw_length_error();
        if (__old_size + __n > capacity()) {
            const size_type __len = __old_size + (max)(__old_size, size_type(__n)) + 1;
            _CharT* __new_start = this->_M_end_of_storage.allocate(__len);
            _CharT* __new_finish = __new_start;
            __new_finish = uninitialized_copy(this->_M_start, this->_M_finish, __new_start);
            __new_finish = uninitialized_copy(__first, __last, __new_finish);
            _M_construct_null(__new_finish);
            _Destroy(this->_M_start, this->_M_finish + 1);
            this->_M_deallocate_block();
            this->_M_start = __new_start;
            this->_M_finish = __new_finish;
            this->_M_end_of_storage._M_data = __new_start + __len;
        } else {
            _ForwardIter __next = __first;
            ++__next;
            uninitialized_copy(__next, __last, this->_M_finish + 1);
            _M_construct_null(this->_M_finish + __n);
            _Traits::assign(*end(), *__first);
            this->_M_finish += __n;
        }
    }
    return *this;
}
#pragma auto_inline(on)

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc> operator+(const _CharT* __s, const basic_string<_CharT, _Traits, _Alloc>& __y)
{
    typedef basic_string<_CharT, _Traits, _Alloc> _Str;
    typedef typename _Str::_Reserve_t _Reserve_t;
    const size_t __n = _Traits::length(__s);
    _Reserve_t __reserve;
    _Str __result(__reserve, __n + __y.size(), __y.get_allocator());
    __result.append(__s, __s + __n);
    __result.append(__y);
    return __result;
}

template basic_string<char> operator+(const char*, const basic_string<char>&);

}
