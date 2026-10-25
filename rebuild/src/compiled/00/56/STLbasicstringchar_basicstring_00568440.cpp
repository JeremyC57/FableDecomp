// std::basic_string<char>::basic_string(const char*, const allocator<char>&) -- construct a string from a C string: empty storage, then copy [s, s + strlen(s)) into a fresh block
// Leaf: ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBDABV?$allocator@D@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00568440, 65 bytes, `__fastcall`, `ret 8`, returns `this`.
// Identity: the manifest label (basic_string<char, char_traits<char>, allocator<char> >::basic_string) agrees with
//   the body: STLport `_String_base(const allocator&)` (start, finish, end_of_storage zeroed), the strlen intrinsic,
//   then `_M_range_initialize(first, last, const forward_iterator_tag&)` @ 0x005672D0 (out of line: allocates
//   size + 1 through malloc, copies the range, terminates). 11 callers (0x005611EF, 0x0084089E .. 0x008F9900).
// Shape: the iterator-tag temporary is homed in the argument slot of `s` (already in edx) and passed by
//   address; `s + strlen(s)` sits in the same argument list as the tag, so its `add` follows the tag push.
#include <string.h>
#pragma intrinsic(strlen)

namespace std {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _CharT> struct char_traits {
    static unsigned int length(const char* __s) { return strlen(__s); }
};

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
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _Traits traits_type;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;
    basic_string(const _CharT* __s, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + traits_type::length(__s), forward_iterator_tag());
    }
    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);   // 0x005672D0
};

template basic_string<char>::basic_string(const char*, const allocator<char>&);

}