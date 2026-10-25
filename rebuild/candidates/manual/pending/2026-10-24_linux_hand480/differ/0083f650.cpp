// Grade: DIFFER(497v442) -- inline-budget lane gap: retail expands the STLport range initialise (0x00567D00) inline at the loop copy and at `return ""` but calls it out of line at `return line` (zero the result's three words, push tag/finish/start, `call 0x567D00`); VC7.1 RTM inlines all three sites here, which also moves line's size from ebx to a spilled ebp. Everything else (loop, StartsWith 0x83EE10, inline find(":") via search 0x567120, inline find_first_not_of via __find_if 0x841900, substr 0x569330, the frees, the puts/abort tail) lines up instruction for instruction. Tried: in-class vs out-of-class _M_range_initialize/ctors/find, STLport distance/category/_Destroy layers, auto-inline StartsWith, `#pragma inline_depth(1..3)` around `return line` (no effect; 0 outlines the whole copy ctor). Only placement-new _Construct (null checks retail lacks) tipped site 2 out of line.
// LHNetworkLib::LHHttpHeaders::GetHeader(const string& name) -- the value of the first header line that starts (case-insensitively) with name: the text after the first ':' with leading spaces skipped, the whole line when it has no ':' past column 0, or "" when no line matches
// Leaf: ?GetHeader@LHHttpHeaders@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083F650, 442 bytes, thiscall (hidden result pointer, then name), `ret 8`.
// Identity: Ego_r.pdb ?GetHeader@LHHttpHeaders@LHNetworkLib@@QAE?AV?$basic_string@...@std@@ABV34@@Z (the debug build's
//   0x0084E820). The catalog's "ProcessTextWithRegex" is descriptive.
// Callees: malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14 (the inline string copies and destructor),
//   LHNetworkLib::StartsWith 0x0083EE10 (landed), search<const char*, const char*, _Eq_traits> 0x00567120 (find(":")),
//   __find_if<const char*, _Not_within_traits> 0x00841900 (find_first_not_of(" "), this chunk), basic_string::substr
//   0x00569330 (landed), _M_range_initialize(f, l, const forward_iterator_tag&) 0x00567D00 (`return line`'s copy), puts
//   0x00BFEAF2 + abort 0x00BFEAEC (STLport's length error with exceptions off, inline after the first copy).
// Data: ":" 0x0124C528, " " 0x01258C1C, "basic_string" 0x0124C5B0.
// Layout: LHHttpHeaders {vector<string> HdrDocumentHeader +0, string HeaderString +0xc} (landed Reset 0x0083FBC0).
// Shape: `for (it ...) { string line = *it; if (StartsWith(line, name)) { int colon = line.find(":"); if (colon > 0)
//   return line.substr(line.find_first_not_of(" ", colon + 1)); return line; } } return "";` -- the loop's copy is
//   the inline STLport range initialise (the length-error arm inline), `return line` calls it out of line, `return ""`
//   folds it (malloc(1), terminator). line's block pointer stays in esi across StartsWith and substr because both
//   bodies (and toLowerCase under StartsWith) are in the TU and never store the string's address.
#include <string.h>
#pragma intrinsic(memcmp, strlen)

extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6
extern "C" int __cdecl puts(const char* text);                                        // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                                // 0x00BFEAEC
extern "C" int __cdecl tolower(int c);                                                // 0x00BFED10

namespace std {

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _CharT> class char_traits;
template <> class char_traits<char> {
public:
    typedef char char_type;
    static void assign(char& __c1, const char& __c2) { __c1 = __c2; }
    static bool eq(const char& __c1, const char& __c2) { return __c1 == __c2; }
    static unsigned int length(const char* __s) { return strlen(__s); }
    static int compare(const char* __s1, const char* __s2, unsigned int __n) { return memcmp(__s1, __s2, __n); }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}
inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Traits> struct _Eq_traits
    : public binary_function<typename _Traits::char_type, typename _Traits::char_type, bool> {
    typedef typename _Traits::char_type char_type;
    bool operator()(const char_type& __x, const char_type& __y) const { return _Traits::eq(__x, __y); }
};
template <class _Traits> struct _Eq_char_bound {
    typedef typename _Traits::char_type char_type;
    char_type __val;
    _Eq_char_bound(char_type __c) : __val(__c) { }
    bool operator()(const char_type& __c) const { return _Traits::eq(__c, __val); }
};

template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
_ForwardIter1 __fastcall search(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2,
                                _ForwardIter2 __last2, _BinaryPred __predicate);                       // 0x00567120
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&);                            // 0x00841900
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}

template <class _Traits> struct _Not_within_traits {
    typedef const typename _Traits::char_type* _Pointer;
    const _Pointer _M_first;
    const _Pointer _M_last;
    _Not_within_traits(_Pointer __f, _Pointer __l) : _M_first(__f), _M_last(__l) { }
    bool operator()(const typename _Traits::char_type& __x) const
    {
        return find_if((_Pointer)_M_first, (_Pointer)_M_last, _Eq_char_bound<_Traits>(__x)) == (_Pointer)_M_last;
    }
};

template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    unsigned int max_size() const { return ((unsigned int)-1 / sizeof(_Tp)) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }
        else
            _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_throw_length_error() const { puts("basic_string"); abort(); }             // (0x00565A30 out of line)
    void _M_throw_out_of_range() const { puts("basic_string"); abort(); }             // (0x00565360 out of line)
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _String_base(const allocator_type& __a, unsigned int __n)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {
        _M_allocate_block(__n);
    }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;
    enum { npos = -1, _DEFAULT_SIZE = 8 };

    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string() : _String_base<_CharT, _Alloc>(allocator_type(), _DEFAULT_SIZE) { _M_terminate_string(); }
    basic_string(const _Self& __s) : _String_base<_CharT, _Alloc>(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
    basic_string(const _CharT* __s, const allocator_type& __a = allocator_type()) : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + _Traits::length(__s));
    }
    basic_string(const _CharT* __f, const _CharT* __l, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__f, __l);
    }
    ~basic_string() { }

    size_type size() const { return this->_M_finish - this->_M_start; }
    const _CharT* data() const { return this->_M_start; }
    const _CharT& operator[](size_type __n) const { return *(this->_M_start + __n); }
    void reserve(size_type __res_arg = 0);                                             // 0x00777DB0
    void push_back(_CharT __c)
    {
        if (this->_M_finish + 1 == this->_M_end_of_storage._M_data)
            reserve(size() + (max)(size(), (size_type)1));
        _M_construct_null(this->_M_finish + 1);
        _Traits::assign(*(this->_M_finish), __c);
        ++this->_M_finish;
    }
    _Self& operator+=(_CharT __c) { push_back(__c); return *this; }

    size_type find(const _CharT* __s, size_type __pos = 0) const { return find(__s, __pos, _Traits::length(__s)); }
    size_type find(const _CharT* __s, size_type __pos, size_type __n) const
    {
        if (__pos + __n > size())
            return npos;
        const _CharT* __result = search((const _CharT*)this->_M_start + __pos, (const _CharT*)this->_M_finish, __s,
                                        __s + __n, _Eq_traits<_Traits>());
        return __result != this->_M_finish ? __result - this->_M_start : npos;
    }
    size_type find_first_not_of(const _CharT* __s, size_type __pos = 0) const
    {
        return find_first_not_of(__s, __pos, _Traits::length(__s));
    }
    size_type find_first_not_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
        if (__pos > size())
            return npos;
        const _CharT* __result = find_if((const _CharT*)this->_M_start + __pos, (const _CharT*)this->_M_finish,
                                         _Not_within_traits<_Traits>((const _CharT*)__s, (const _CharT*)__s + __n));
        return __result != this->_M_finish ? __result - this->_M_start : npos;
    }
    __declspec(noinline) _Self substr(size_type __pos = 0, size_type __n = npos) const     // 0x00569330
    {
        if (__pos > size())
            this->_M_throw_out_of_range();
        return _Self(this->_M_start + __pos, this->_M_start + __pos + (min)(__n, size() - __pos));
    }

    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&);  // 0x00567D00
    void _M_range_initialize(const _CharT* __f, const _CharT* __l)
    {
        _M_range_initialize(__f, __l, forward_iterator_tag());
    }
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
};

template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_range_initialize(const _CharT* __f, const _CharT* __l,
                                                                const forward_iterator_tag&)
{
    int __n = __l - __f;
    this->_M_allocate_block(__n + 1);
    this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
    _M_terminate_string();
}

template <class _CharT, class _Traits, class _Alloc>
inline bool __fastcall operator==(const basic_string<_CharT, _Traits, _Alloc>& __x,
                                  const basic_string<_CharT, _Traits, _Alloc>& __y)
{
    return __x.size() == __y.size() && _Traits::compare(__x.data(), __y.data(), __x.size()) == 0;
}

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {                   // STLport, 12 bytes
public:
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

typedef basic_string<char> string;
}

namespace LHNetworkLib {

#pragma auto_inline(off)
std::string __fastcall toLowerCase(const std::string& text)                          // 0x0083EBD0
{
    std::string result;
    int length = text.size();
    for (int i = 0; i < length; ++i)
        result += (char)tolower(text[i]);
    return result;
}

bool __fastcall StartsWith(const std::string& text, const std::string& prefix)       // 0x0083EE10 (landed)
{
    return toLowerCase(text).substr(0, prefix.size()) == toLowerCase(prefix);
}
#pragma auto_inline(on)

class LHHttpHeaders {
public:
    std::vector<std::string> HdrDocumentHeader;                                         // +0x00
    std::string              HeaderString;                                              // +0x0c
    std::string GetHeader(const std::string& name);
};

std::string LHHttpHeaders::GetHeader(const std::string& name)
{
    for (std::vector<std::string>::iterator it = HdrDocumentHeader.begin(); it != HdrDocumentHeader.end(); ++it) {
        std::string line = *it;
        if (StartsWith(line, name)) {
            int colon = line.find(":");
            if (colon > 0)
                return line.substr(line.find_first_not_of(" ", colon + 1));
            return line;
        }
    }
    return "";
}

}
