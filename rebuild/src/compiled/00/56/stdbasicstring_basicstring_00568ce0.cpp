// std::basic_string::basic_string(const basic_string& s) -- STLport narrow-string copy constructor: empty base, then allocate and copy s's characters [start, finish) with a terminator
// Leaf: ??0?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00568CE0, 42 bytes, thiscall, `ret 4`.
// Identity: STLport 4.x basic_string<char> copy constructor of the networking module's strings (the landed
//   operator+ 0x00569690 / vector<string> rows inline the same copy around 0x00567D00); this out-of-line COMDAT has no
//   direct caller in the image (xref: 0 calls / 0 references). Namespace `std` as the landed 0x00567D00 siblings.
// Callees: basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&) 0x00567D00
//   (landed; `ret 0xC`).
// Layout: STLport basic_string = _String_base { _M_start +0, _M_finish +4, _M_end_of_storage (alloc proxy) +8 }.
// Shape: STLport verbatim `basic_string(const _Self& __s) : _String_base<_CharT,_Alloc>(__s.get_allocator())
//   { _M_range_initialize(__s._M_start, __s._M_finish); }` -- the three zero stores, the empty iterator tag homed in
//   the `s` argument slot (`lea eax,[esp+8]`, pushed first), then s is reloaded and finish / start pushed.
// Label correction: catalogued as `vector<long>::vector<int>` (propagated); the body is the STLport basic_string<char>
//   copy constructor.
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
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    typedef _Alloc allocator_type;

    allocator_type get_allocator() const { return (const allocator_type&)this->_M_end_of_storage; }

    basic_string(const _Self& __s) : _String_base<_CharT, _Alloc>(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }

    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);   // 0x00567D00
    void _M_range_initialize(const _CharT* __f, const _CharT* __l)
    {
        _M_range_initialize(__f, __l, random_access_iterator_tag());
    }
};

template basic_string<char>::basic_string(const basic_string<char>&);

}