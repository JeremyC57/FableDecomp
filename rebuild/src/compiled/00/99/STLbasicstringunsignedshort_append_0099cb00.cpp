// std::basic_string<unsigned short>::append(size_type n, unsigned short c) -- append n copies of c (length check, grow to size + max(size, n), fill, terminate)
// Bounds: int3-after-noreturn row: RELOCATION_MATCH against the 145-byte row read straight from Fable.exe (byte 144 is the 0xCC VC7.1 emits after the final noreturn _M_throw_length_error call; the chunk row and q.py strip it as padding). Land against an N+1 oracle row (GOTCHAS: int3-after-noreturn rows).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099CB00, 144 bytes, thiscall `ret 8`, returns *this. The function ends in the
// noreturn length-error call.
// Callees: basic_string<unsigned short>::reserve @ 0x0099C990, uninitialized_fill_n<unsigned short*, unsigned int,
// unsigned short> @ 0x005123A0 (an out-of-line /Gr copy: first in ecx, count in edx, &value pushed, `ret 4`),
// basic_string<unsigned short>::_M_throw_length_error() const @ 0x0099C550 (noreturn, laid out of line at the end).
// Callers: basic_string<unsigned short>::resize 0x0099CD60 (landed). The landed CWideString::operator+=(wchar_t)
// 0x0099B9A0 holds the same body inlined for n = 1.
// Label correction: the manifest's "std_vector_resize" is propagated; the body is STLport basic_string::append(n, c).
// Shape: STLport 4.x verbatim -- `max` by reference over size() (spilled to the `push ecx` frame word) and n's own
// argument slot; the fill reads c through its argument slot; _M_construct_null(_M_finish + n) then
// `_Traits::assign(*end(), c)` re-read _M_finish after the fill call.
// Leaf: ?append@?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@QAEAAV12@IG@Z
namespace std {

typedef unsigned int size_t;

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);   // 0x005123A0

template<class _CharT> class char_traits {
public:
    static void assign(_CharT& __c1, const _CharT& __c2) { __c1 = __c2; }
};

template<class _Tp> class allocator { };

template<class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {
public:
    typedef _CharT* pointer;
    typedef _CharT* iterator;
    typedef size_t  size_type;

    pointer _M_start;            // +0x00
    pointer _M_finish;           // +0x04
    pointer _M_end_of_storage;   // +0x08

    iterator end() { return _M_finish; }
    size_type size() const { return _M_finish - _M_start; }
    size_type max_size() const { return size_type(-1) / sizeof(_CharT) - 1; }
    size_type capacity() const { return (_M_end_of_storage - _M_start) - 1; }
    __declspec(noreturn) void _M_throw_length_error() const;   // 0x0099C550
    void _M_construct_null(pointer __p) { *__p = 0; }
    void reserve(size_type __res_arg);                         // 0x0099C990

    basic_string& append(size_type __n, _CharT __c);
};

template<class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::append(size_type __n, _CharT __c)
{
    if (__n > max_size() || size() > max_size() - __n)
        _M_throw_length_error();
    if (size() + __n > capacity())
        reserve(size() + (max)(size(), __n));
    if (__n > 0) {
        uninitialized_fill_n(_M_finish + 1, __n - 1, __c);
        _M_construct_null(_M_finish + __n);
        _Traits::assign(*end(), __c);
        _M_finish += __n;
    }
    return *this;
}

template basic_string<unsigned short>& basic_string<unsigned short>::append(size_type, unsigned short);

}