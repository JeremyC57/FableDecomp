// std::basic_string::basic_string(const unsigned short* s, unsigned int n, const allocator& a) -- STLport wide-string constructor from a character pointer and a count: empty base, then allocate and copy [s, s + n) with a terminator
// Leaf: ??0?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@QAE@PBGIABV?$allocator@G@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099CA40, 44 bytes, thiscall, `ret 0xC`.
// Identity: STLport 4.x basic_string<unsigned short>(const _CharT* s, size_type n, const allocator_type& a) -- the
//   (pointer, count) constructor that the landed CWideString::AllocStringData 0x0099B3C0 inlines into its
//   (text, length) arm; this out-of-line COMDAT has no direct caller in the image (xref: 0 calls / 0 references).
// Callees: basic_string<unsigned short>::_M_range_initialize(const unsigned short*, const unsigned short*,
//   const forward_iterator_tag&) 0x0099C670 (landed; `ret 0xC`).
// Layout: STLport basic_string = _String_base { _M_start +0, _M_finish +4, _M_end_of_storage (alloc proxy) +8 }.
// Shape: STLport verbatim `basic_string(const _CharT* __s, size_type __n, const allocator_type& __a =
//   allocator_type()) : _String_base<_CharT,_Alloc>(__a) { _M_range_initialize(__s, __s + __n); }` -- the three zero
//   stores, then the pointer-iterator dispatch's empty random_access_iterator_tag temporary is homed in the `s`
//   argument slot (`lea eax,[esp+8]`, pushed first), s is reloaded after the push and the end is `s + n*2`.
// Label correction: catalogued as `vector<long>::vector<int>` (propagated); the body is the STLport wide
//   basic_string (pointer, count) constructor.
namespace std {

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _CharT> class char_traits { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
    _String_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;

    basic_string(const _CharT* __s, size_type __n, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + __n);
    }

    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);   // 0x0099C670
    void _M_range_initialize(const _CharT* __f, const _CharT* __l)
    {
        _M_range_initialize(__f, __l, random_access_iterator_tag());
    }
};

template basic_string<unsigned short>::basic_string(const unsigned short*, unsigned int, const allocator<unsigned short>&);

}