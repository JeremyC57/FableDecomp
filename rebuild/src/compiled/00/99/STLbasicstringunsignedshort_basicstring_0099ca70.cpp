// _STL::basic_string<unsigned short>::basic_string(size_type n, unsigned short c, const allocator_type& a) -- build a wide string of n copies of c
// Bounds: int3-after-noreturn row: RELOCATION_MATCH against the 118-byte row read straight from Fable.exe (byte 117 is the 0xCC VC7.1 emits after the final noreturn _M_throw_length_error call; the chunk row and q.py strip it as padding). Land against an N+1 oracle row (GOTCHAS: int3-after-noreturn rows).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099CA70, 117 bytes, thiscall `ret 0Ch`, returns this. The function ends in the
// noreturn length-error call.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate), basic_string<unsigned short>::_M_throw_length_error() const
// @ 0x0099C550 (noreturn, `mov ecx,esi`).
// Label correction: the manifest's "WideString_Allocate" is propagated; the body is the STLport fill constructor.
// Shape: STLport 4.x: _String_base(a, n + 1) zero-stores the three pointers and runs _M_allocate_block (`n <= max_size()
// + 1 && n > 0`, else the length error); _M_finish = uninitialized_fill_n(_M_start, n, c) -- the trivial fill_n loop over
// the by-value c, which VC7.1 turns into `rep stosd ; adc ecx,ecx ; rep stosw` -- then _M_terminate_string().
// Leaf: ??0?$basic_string@GV?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAE@IGABV?$allocator@G@1@@Z
namespace _STL {

typedef unsigned int size_t;

extern "C" void* __cdecl malloc(size_t size);                              // 0x00BFEA0E

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return fill_n(__first, __n, __x);
}

template<class _CharT> class char_traits { };

template<class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template<class _Tp, class _Alloc> class _String_base {
public:
    typedef size_t size_type;
    _Tp* _M_start;              // +0x00
    _Tp* _M_finish;             // +0x04
    _Tp* _M_end_of_storage;     // +0x08

    size_type max_size() const { return size_type(-1) / sizeof(_Tp) - 1; }
    __declspec(noreturn) void _M_throw_length_error() const;   // 0x0099C550

    void _M_allocate_block(size_type __n)
    {
        if (__n <= max_size() + 1 && __n > 0) {
            _Alloc __a;
            _M_start = __a.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage = _M_start + __n;
        } else
            _M_throw_length_error();
    }

    _String_base(const _Alloc&, size_type __n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_allocate_block(__n);
    }
};

template<class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : public _String_base<_CharT, _Alloc> {
public:
    typedef size_t size_type;
    typedef _Alloc allocator_type;

    void _M_construct_null(_CharT* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }

    basic_string(size_type __n, _CharT __c, const allocator_type& __a);
};

template<class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc>::basic_string(size_type __n, _CharT __c, const allocator_type& __a)
    : _String_base<_CharT, _Alloc>(__a, __n + 1)
{
    this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __c);
    _M_terminate_string();
}

template basic_string<unsigned short>::basic_string(size_type, unsigned short, const allocator<unsigned short>&);

}