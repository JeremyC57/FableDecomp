// LHNetworkLib::StartsWith(const string& text, const string& prefix) -- case-insensitive prefix test: the lower-cased text cut to the prefix length equals the lower-cased prefix
// Leaf: ?StartsWith@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EE10, 194 bytes, __fastcall (text in ecx, prefix in edx), bool in al, `ret`.
// Identity: Ego_r.pdb ?StartsWith@LHNetworkLib@@YI_NABV?$basic_string@...@std@@0@Z (YI: __fastcall, _N: bool), the third of
//   the Ego_r run toLowerCase / toUpperCase / StartsWith; TLC's toLowerCase is the callee 0x0083EBD0 (YI, hidden return in
//   ecx, the string in edx). Parameter names are descriptive. The manifest label "HTTP_CompareStrings" is an autoname.
// Callees: LHNetworkLib::toLowerCase 0x0083EBD0 (twice), STLport basic_string<char>::_M_range_initialize(const char*,
//   const char*, const forward_iterator_tag&) 0x00567D00 (the inline substr's result), free 0x00BFEA14 (the three
//   temporaries).
// Shape: `return toLowerCase(text).substr(0, prefix.size()) == toLowerCase(prefix);` -- prefix.size() spilled first (the
//   STLport `min(__n, size() - __pos)` takes it by reference), substr inline (the `__pos > size()` test folds away; the
//   out-of-range thrower is `__declspec(noreturn)`, which lets substr inline), operator== inline (size test, then the
//   memcmp intrinsic as `repe cmpsb`), the bool kept on the frame while the three temporaries are freed. Two shape
//   details: the free inlines are `__fastcall` (/Gr: STLport's `_STLP_CALL` is empty), which makes VC7.1 evaluate the
//   left operand of == first (a default-convention operator== calls toLowerCase(prefix) first); and the result string
//   is built from (start + pos, len) through the (const char*, size_type) constructor, whose `__s + __n` sits in the
//   same argument list as the iterator tag, so the add follows the tag push as in retail (the (first, last)
//   constructor computes the end pointer before the tag: one swapped instruction, 194 v 194).
#include <string.h>
#pragma intrinsic(memcmp)
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

namespace std {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _Tp> inline const _Tp& __fastcall min(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct char_traits_char {
    static int compare(const char* __s1, const char* __s2, unsigned int __n) { return memcmp(__s1, __s2, __n); }
};

class _String_base {
public:
    typedef allocator<char> allocator_type;
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
    ~_String_base() { _M_deallocate_block(); }
};

class basic_string : public _String_base {
public:
    typedef unsigned int size_type;
    typedef basic_string _Self;
    __declspec(noreturn) void _M_throw_out_of_range() const;                     // 0x00565360 (not reached: pos is 0)
    void _M_range_initialize(const char* __f, const char* __l, const forward_iterator_tag&);      // 0x00567D00
    void _M_range_initialize(const char* __f, const char* __l) { _M_range_initialize(__f, __l, forward_iterator_tag()); }

    basic_string(const char* __s, size_type __n, const allocator_type& __a = allocator_type()) : _String_base(__a)
    {
        _M_range_initialize(__s, __s + __n, forward_iterator_tag());
    }
    basic_string(const basic_string& __s);
    size_type size() const { return _M_finish - _M_start; }
    const char* data() const { return _M_start; }
    _Self substr(size_type __pos = 0, size_type __n = (size_type)-1) const
    {
        if (__pos > size())
            _M_throw_out_of_range();
        size_type __len = min(__n, size() - __pos);    // named first, as the landed out-of-line substr 0x00569330
        return _Self(_M_start + __pos, __len);
    }
};

inline bool __fastcall operator==(const basic_string& __x, const basic_string& __y)   // /Gr: _STLP_CALL is empty
{
    return __x.size() == __y.size() && char_traits_char::compare(__x.data(), __y.data(), __x.size()) == 0;
}

}

typedef std::basic_string string;

namespace LHNetworkLib {
string __fastcall toLowerCase(const string& text);                                // 0x0083EBD0

bool __fastcall StartsWith(const string& text, const string& prefix)
{
    return toLowerCase(text).substr(0, prefix.size()) == toLowerCase(prefix);
}
}