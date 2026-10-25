// std::basic_string<unsigned short>::push_back(unsigned short c) -- append one wide character through the inline append(1, c)
// Bounds: int3-after-noreturn row: RELOCATION_MATCH against the 141-byte row read straight from Fable.exe (byte 140 is the 0xCC VC7.1 emits after the final noreturn _M_throw_length_error call; the chunk row and q.py strip it as padding). Land against an N+1 oracle row (GOTCHAS: int3-after-noreturn rows).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099CC80, 140 bytes, thiscall . The function ends in the noreturn
// length-error call.
// Callees: basic_string<unsigned short>::reserve @ 0x0099C990, uninitialized_fill_n<unsigned short*, unsigned int,
// unsigned short> @ 0x005123A0 (/Gr copy: first in ecx, count in edx = 0, &value pushed),
// basic_string<unsigned short>::_M_throw_length_error() const @ 0x0099C550 (noreturn, laid out of line at the end).
// Label correction: the manifest's "CTCElectrocutedEffect_PushEffect" is propagated; the body is the STLport wide
// string push_back = append(1, c) (the out-of-line append is 0x0099CB00 in the same TU).
// Shape: append(1, c) expanded inline (this inline copy spells the landed 0x0099B9A0 form: `fill = c; count = n;`
// -- the out-of-line 0x0099CB00 body differs with them, 154v145): c is copied into a frame word (the fill reads it there) and the constant count
// 1 is homed in c's dead argument slot (max(size(), n) takes its address).
// Leaf: ?push_back@?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@QAEXG@Z
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
    void push_back(_CharT __c) { append(1, __c); }
};

template<class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::append(size_type __n, _CharT __c)
{
    _CharT __fill = __c;
    size_type __count = __n;
    if (__count > max_size() || size() > max_size() - __count)
        _M_throw_length_error();
    if (size() + __count > capacity())
        reserve(size() + (max)(size(), __count));
    if (__count > 0) {
        uninitialized_fill_n(_M_finish + 1, __count - 1, __fill);
        _M_construct_null(_M_finish + __count);
        _Traits::assign(*end(), __c);
        _M_finish += __count;
    }
    return *this;
}

template void basic_string<unsigned short>::push_back(unsigned short);

}