// std::basic_string<unsigned short>::_M_range_initialize(const unsigned short* f, const unsigned short* l, const forward_iterator_tag&) -- allocate (l-f)+1 wide characters, copy the range and terminate it
// Grade: DIFFER(98v97) -- int3-after-noreturn row: RELOCATION_MATCH against the 98-byte row read straight from Fable.exe (byte 97 is the 0xCC VC7.1 emits after the final noreturn _M_throw_length_error call; the chunk row and q.py strip it as padding). Land against an N+1 oracle row (GOTCHAS: int3-after-noreturn rows).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099C670, 97 bytes, thiscall `ret 0Ch` (the empty forward-iterator tag is the 3rd
// argument). The function ends in the noreturn length-error call (nothing follows it).
// Callees: malloc @ 0x00BFEA0E (allocator::allocate; the n != 0 test folds away under n > 0), memmove @ 0x00BFEAE6
// (__copy_trivial), basic_string<unsigned short>::_M_throw_length_error() const @ 0x0099C550 (landed BasicStringRaise,
// puts + abort with exceptions off; `mov ecx,esi`, noreturn).
// Label correction: the manifest's "CCharString_AllocStringData" is propagated; the body is the wide sibling of the
// landed CNetString_Retail::RangeInitialize 0x00567D00 (2-byte element: `sar 1`, max_size 0x7FFFFFFE, 16-bit NUL).
// Shape: STLport 4.x verbatim: _M_allocate_block(n + 1) (`n <= max_size() + 1 && n > 0`, else the length error),
// finish = uninitialized_copy(f, l, start) through __copy_trivial (the byte distance l - f is kept for memmove and the
// end pointer), then _M_construct_null(finish).
// Leaf: ?_M_range_initialize@?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@
namespace std {

typedef unsigned int size_t;

extern "C" void* __cdecl malloc(size_t size);                              // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);    // 0x00BFEAE6

struct forward_iterator_tag { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template<class _CharT> class char_traits { };

template<class _Tp> class allocator {
public:
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template<class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {
public:
    typedef _CharT* pointer;
    typedef size_t  size_type;

    pointer _M_start;            // +0x00
    pointer _M_finish;           // +0x04
    pointer _M_end_of_storage;   // +0x08

    size_type max_size() const { return size_type(-1) / sizeof(_CharT) - 1; }
    __declspec(noreturn) void _M_throw_length_error() const;   // 0x0099C550

    void _M_allocate_block(size_type __n)
    {
        if (__n <= max_size() + 1 && __n > 0) {
            _Alloc a;
            _M_start = a.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage = _M_start + __n;
        } else
            _M_throw_length_error();
    }
    void _M_construct_null(pointer __p) { *__p = 0; }

    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);
};

template<class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_range_initialize(const _CharT* __f, const _CharT* __l,
                                                                 const forward_iterator_tag&)
{
    int __n = __l - __f;
    _M_allocate_block(__n + 1);
    _M_finish = (pointer)__copy_trivial(__f, __l, _M_start);
    _M_construct_null(_M_finish);
}

template void basic_string<unsigned short>::_M_range_initialize(const unsigned short*, const unsigned short*,
                                                                const forward_iterator_tag&);

}
