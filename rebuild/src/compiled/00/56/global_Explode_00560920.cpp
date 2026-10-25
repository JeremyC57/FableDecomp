// _global::Explode(basic_string& text, const basic_string& separator, vector<basic_string>& out) -- splits the text at every separator, appending each piece before a separator to out; returns out's size
// Leaf: ?Explode@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00560920, 656-byte row, __fastcall (ecx = text, edx = separator, [esp+4] = out),
//   `ret 4`, returns the vector's element count.
// Identity: Ego_r.pdb ?Explode@?A0xf010c0d0@@YIHAAV?$basic_string@D...@std@@ABV23@AAV?$vector@V?$basic_string@D...@Z
//   -- an anonymous-namespace free function of the scoreboard/crypto TU; its one caller is
//   CTCHeroOnlineScoreboard::GetDecryptedDocument 0x00560BB0 (Explode(text, "\r\n", lines)). The catalog's
//   "DecryptResponse_ParseElements" is descriptive.
// Callees: search<const char*, const char*, _Eq_traits> 0x00567120, __uninitialized_copy<string> 0x00568E70,
//   basic_string<char>::_M_range_initialize<char*> 0x00567D00, _M_throw_out_of_range 0x00565360,
//   _M_throw_length_error 0x00565A30, malloc 0x00BFEA0E, memmove 0x00BFEAE6, free 0x00BFEA14, puts 0x00BFEAF2,
//   abort 0x00BFEAEC; "basic_string" 0x0124C5B0.
// Shape: STLport 4.6 verbatim: `find(sep, pos)` (the inline three-argument find over search with the empty
//   _Eq_traits predicate), `substr(pos, found - pos)` into a named piece, `push_back(piece)` with
//   _M_insert_overflow inlined, then `pos = found + sep.size()`; the piece is destroyed at the end of each pass.
//   The string throw helpers are STLport's out-of-class `_String_base` members (declared noreturn; the length
//   error forwards to the inline `__stl_throw_length_error` = puts + abort): VC7.1 expands the length error once
//   (the piece's allocation) and calls 0x00565A30 from the push_back copy; the out-of-range throw is a call.
#include <string.h>
#pragma intrinsic(strlen)

extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6
extern "C" int __cdecl puts(const char* text);                                        // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                                 // 0x00BFEAEC

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };

template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _CharT> class char_traits {
public:
    static bool eq(const _CharT& __c1, const _CharT& __c2) { return __c1 == __c2; }
    static unsigned int length(const _CharT* __s) { return strlen(__s); }
};

template <class _Arg1, class _Arg2, class _Result> struct binary_function { };

template <class _Traits> struct _Eq_traits : public binary_function<char, char, bool> {
    bool operator()(const char& __x, const char& __y) const { return _Traits::eq(__x, __y); }
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

inline void __stl_throw_length_error(const char* __msg) { puts(__msg); abort(); }

template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
__declspec(noinline) _ForwardIter1 __fastcall search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                                                     _ForwardIter2 __first2, _ForwardIter2 __last2,
                                                     _BinaryPred __predicate)
{                                                                                      // 0x00567120
    if (__first1 == __last1 || __first2 == __last2)
        return __first1;
    _ForwardIter2 __tmp(__first2);
    ++__tmp;
    if (__tmp == __last2) {
        while (__first1 != __last1 && !__predicate(*__first1, *__first2))
            ++__first1;
        return __first1;
    }
    _ForwardIter2 __p1, __p;
    __p1 = __first2; ++__p1;
    while (__first1 != __last1) {
        while (__first1 != __last1) {
            if (__predicate(*__first1, *__first2))
                break;
            ++__first1;
        }
        while (__first1 != __last1 && !__predicate(*__first1, *__first2))
            ++__first1;
        if (__first1 == __last1)
            return __last1;
        __p = __p1;
        _ForwardIter1 __current = __first1;
        if (++__current == __last1)
            return __last1;
        while (__predicate(*__current, *__p)) {
            if (++__p == __last2)
                return __first1;
            if (++__current == __last1)
                return __last1;
        }
        ++__first1;
    }
    return __first1;
}

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
    __declspec(noreturn) void _M_throw_length_error() const;                            // 0x00565A30
    __declspec(noreturn) void _M_throw_out_of_range() const;                            // 0x00565360
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _Tp, class _Alloc> void _String_base<_Tp, _Alloc>::_M_throw_length_error() const
{
    __stl_throw_length_error("basic_string");
}

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _Traits traits_type;
    typedef unsigned int size_type;
    typedef const _CharT* const_pointer;
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;
    enum { npos = -1 };

    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string(const _Self& __s) : _String_base<_CharT, _Alloc>(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
    template <class _InputIterator>
    basic_string(_InputIterator __f, _InputIterator __l, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_initialize_dispatch(__f, __l, __false_type());
    }
    ~basic_string() { }

    size_type size() const { return this->_M_finish - this->_M_start; }
    size_type find(const _Self& __s, size_type __pos = 0) const { return find(__s._M_start, __pos, __s.size()); }
    size_type find(const _CharT* __s, size_type __pos, size_type __n) const
    {
        if (__pos + __n > size())
            return npos;
        else {
            const const_pointer __result =
                search((const _CharT*)this->_M_start + __pos, (const _CharT*)this->_M_finish,
                       __s, __s + __n, _Eq_traits<_Traits>());
            return __result != this->_M_finish ? __result - this->_M_start : npos;
        }
    }
    _Self substr(size_type __pos = 0, size_type __n = npos) const
    {
        if (__pos > size())
            this->_M_throw_out_of_range();
        return _Self(this->_M_start + __pos, this->_M_start + __pos + (min)(__n, size() - __pos));
    }

    template <class _InIter>
    void _M_initialize_dispatch(_InIter __f, _InIter __l, const __false_type&) { _M_range_initialize(__f, __l); }
    template <class _InputIter>
    void _M_range_initialize(_InputIter __f, _InputIter __l) { _M_range_initialize(__f, __l, forward_iterator_tag()); }
    template <class _ForwardIter>
    void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&)
    {                                                                                  // 0x00567D00 for char*
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                      // 0x00568E70
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                      // 0x00568F10
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

}

typedef std::basic_string<char> CNetString;
typedef std::vector<CNetString> CNetStringVector;

namespace {
int __fastcall Explode(CNetString& text, const CNetString& separator, CNetStringVector& out)
{
    CNetString::size_type pos = 0;
    CNetString::size_type found;
    while ((found = text.find(separator, pos)) != CNetString::npos) {
        CNetString piece = text.substr(pos, found - pos);
        out.push_back(piece);
        pos = found + separator.size();
    }
    return out.size();
}
}

int (__fastcall *g_keepExplode)(CNetString&, const CNetString&, CNetStringVector&) =
    &Explode;   // keeps the internal-linkage body emitted; not part of the retail function