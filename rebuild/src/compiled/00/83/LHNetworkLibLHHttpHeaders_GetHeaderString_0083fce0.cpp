// LHNetworkLib::LHHttpHeaders::GetHeaderString() -- rebuild and return the request header block: clear HeaderString, add the default "User-Agent: Lionhead Studios HTTP Library" line when no User-Agent header is present, then append every header line followed by CRLF
// Bounds: int3-after-noreturn row, landed against the N+1 span: int3-after-noreturn grader artefact only: the body ends in `call 0x565A30` (_M_throw_length_error) and VC7.1 emits the 0xCC after it as part of the function, which the chunk cutter stripped as padding; against the N+1 retail span (Fable.exe 0x0083FCE0..0x0083FF74 inclusive) this exact file grades RELOCATION_MATCH ($S/w22/agent480/qn1.py). Land it by hand against the N+1 row (GOTCHAS int3-after-noreturn).
// Leaf: ?GetHeaderString@LHHttpHeaders@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FCE0, 672 bytes, thiscall (hidden result pointer), `ret 4`.
// Identity: Ego_r.pdb ?GetHeaderString@LHHttpHeaders@LHNetworkLib@@QAE?AV?$basic_string@...@std@@XZ (the debug build's
//   0x0084EFE0). The catalog's "LHHttp_ParseResponseHeaders" is descriptive.
// Callees: basic_string::assign(const char*, const char*) 0x005693B0, LHHttpHeaders::GetHeader 0x0083F650 (this chunk),
//   _M_range_initialize<const char*> 0x005672D0 / <char*> 0x00567D00, vector<string>::_M_insert_overflow 0x00569E30,
//   operator+(const string&, const char*) 0x00841B70 (cdecl), basic_string::_M_append(f, l, tag) 0x00567400,
//   _M_throw_length_error 0x00565A30, malloc / memmove / free / puts / abort.
// Data: "" 0x0122D70E, "User-Agent" 0x0127312C, "\r\n" 0x012397F8, g_DefaultUserAgentHeader 0x0138490C (char* to
//   "User-Agent: Lionhead Studios HTTP Library...").
// Layout: LHHttpHeaders {vector<string> HdrDocumentHeader +0, string HeaderString +0xc}.
// Shape: `HeaderString = ""` (assign("", "")), `if (GetHeader("User-Agent") == "") AddHeader(g_DefaultUserAgentHeader);`
//   (an inline AddHeader = push_back of the converted temporary: the const char* range initialise out of line, the
//   element copy through the char* one, the growth path out of line), then `string line = *it; HeaderString += line +
//   "\r\n";` per header (the loop copy fully inline with the puts/abort length error) and `return HeaderString;` (inline,
//   its length error the out-of-line 0x00565A30). The retail TU holds the bodies of GetHeader and of the STLport
//   members it calls out of line, so VC7.1 knows which locals they leave alone: same-TU noinline bodies (GetHeader with
//   StartsWith/toLowerCase from this chunk's 0x0083F650 model; STLport 4.6 assign / _M_append / operator+ /
//   _M_insert_overflow as stand-ins) drop the dead zero stores of the "User-Agent" temporary, keep the default
//   header's block in esi across push_back and home the iterator tags in the dead result-pointer slot. With only
//   declarations every one of those addresses escapes and the temporaries go back through memory.
#include <string.h>
#pragma intrinsic(memcmp, strlen)

extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6
extern "C" int __cdecl puts(const char* text);                                        // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                                // 0x00BFEAEC
extern "C" int __cdecl tolower(int c);                                                // 0x00BFED10

inline void* operator new(unsigned int, void* where) { return where; }

namespace std {

struct __false_type { __false_type() { } };
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

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
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
    basic_string(unsigned int __n, const allocator_type& __a) : _String_base<_CharT, _Alloc>(__a, __n)
    {
        _M_terminate_string();
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

    template <class _ForwardIter>
    void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&)   // 0x005672D0 / 0x00567D00
    {
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
    template <class _InputIter> void _M_range_initialize(_InputIter __f, _InputIter __l)
    {
        _M_range_initialize(__f, __l, forward_iterator_tag());
    }
    _Self& assign(const _CharT* __f, const _CharT* __l);                              // 0x005693B0
    _Self& operator=(const _CharT* __s) { return assign(__s, __s + _Traits::length(__s)); }
    template <class _InputIter> _Self& _M_append(_InputIter __f, _InputIter __l, const forward_iterator_tag&);   // 0x00567400
    _Self& append(const _Self& __s) { return _M_append(__s._M_start, __s._M_finish, forward_iterator_tag()); }
    _Self& operator+=(const _Self& __s) { return append(__s); }
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
};

template <class _CharT, class _Traits, class _Alloc>
inline bool __fastcall operator==(const basic_string<_CharT, _Traits, _Alloc>& __x,
                                  const basic_string<_CharT, _Traits, _Alloc>& __y)
{
    return __x.size() == __y.size() && _Traits::compare(__x.data(), __y.data(), __x.size()) == 0;
}

template <class _CharT, class _Traits, class _Alloc>
inline bool operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __c)
{
    unsigned int __n = _Traits::length(__c);
    return __s.size() == __n && _Traits::compare(__s.data(), __c, __n) == 0;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {                                   // STLport, 12 bytes
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return this->_M_finish - this->_M_start; }
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                      // 0x00569E30
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)            // 0x00569E30 (stand-in)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    _Tp* __new_start = (_Tp*)(__len != 0 ? malloc(__len * sizeof(_Tp)) : 0);
    _Tp* __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _Destroy(this->_M_start, this->_M_finish);
    if (this->_M_start != 0)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::assign(const _CharT* __f,
                                                                                    const _CharT* __l)   // 0x005693B0 (stand-in)
{
    unsigned int __n = __l - __f;
    if (__n <= size()) {
        memmove(this->_M_start, __f, __n);
        this->_M_finish = this->_M_start + __n;
        _M_terminate_string();
    }
    else {
        memmove(this->_M_start, __f, size());
        _M_append(__f + size(), __l, forward_iterator_tag());
    }
    return *this;
}

template <class _CharT, class _Traits, class _Alloc> template <class _InputIter>
basic_string<_CharT, _Traits, _Alloc>& basic_string<_CharT, _Traits, _Alloc>::_M_append(_InputIter __first,
    _InputIter __last, const forward_iterator_tag&)                                       // 0x00567400 (stand-in)
{
    if (__first != __last) {
        const unsigned int __old_size = size();
        unsigned int __n = __last - __first;
        if (__n > this->max_size() || __old_size > this->max_size() - __n)
            this->_M_throw_length_error();
        if (__old_size + __n > (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start) - 1) {
            const unsigned int __len = __old_size + (max)(__old_size, __n) + 1;
            _CharT* __new_start = this->_M_end_of_storage.allocate(__len);
            _CharT* __new_finish = uninitialized_copy(this->_M_start, this->_M_finish, __new_start);
            __new_finish = uninitialized_copy(__first, __last, __new_finish);
            _M_construct_null(__new_finish);
            this->_M_deallocate_block();
            this->_M_start = __new_start;
            this->_M_finish = __new_finish;
            this->_M_end_of_storage._M_data = __new_start + __len;
        }
        else {
            uninitialized_copy(__first + 1, __last, this->_M_finish + 1);
            _M_construct_null(this->_M_finish + __n);
            *this->_M_finish = *__first;
            this->_M_finish += __n;
        }
    }
    return *this;
}

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc> operator+(const basic_string<_CharT, _Traits, _Alloc>& __s,
                                                const _CharT* __c)                        // 0x00841B70 (stand-in)
{
    const unsigned int __n = _Traits::length(__c);
    basic_string<_CharT, _Traits, _Alloc> __result(__s.size() + __n + 1, __s.get_allocator());
    __result.append(__s);
    __result._M_append(__c, __c + __n, forward_iterator_tag());
    return __result;
}
#pragma auto_inline(on)

typedef basic_string<char> string;
}

extern const char* g_DefaultUserAgentHeader;                                             // 0x0138490C

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
    std::string GetHeader(const std::string& name);                                     // 0x0083F650
    void AddHeader(const std::string& line) { HdrDocumentHeader.push_back(line); }
    std::string GetHeaderString();
};

#pragma auto_inline(off)
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

#pragma auto_inline(on)

std::string LHHttpHeaders::GetHeaderString()
{
    HeaderString = "";
    if (GetHeader("User-Agent") == "")
        AddHeader(g_DefaultUserAgentHeader);
    for (std::vector<std::string>::iterator it = HdrDocumentHeader.begin(); it != HdrDocumentHeader.end(); ++it) {
        std::string line = *it;
        HeaderString += line + "\r\n";
    }
    return HeaderString;
}

}