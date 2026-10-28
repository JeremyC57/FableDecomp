// LHNetworkLib::toLowerCase(const std::string& text) -- return a lower-case copy of the text: an empty STLport string grows one tolower()'d character at a time (push_back), then is copied out as the result
// Leaf: ?toLowerCase@LHNetworkLib@@YI?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EBD0, 281 bytes, static __fastcall (hidden result slot in ecx, text in edx), plain
//   `ret`; the length-error exit is a tail `jmp 0x00565A30` after the epilogue.
// Identity (proven): Ego_r.pdb ?toLowerCase@LHNetworkLib@@YI?AV?$basic_string@...@std@@ABV23@@Z (0x0084E430) and FableWin.pdb
//   (0x029E5C30) are followed by toUpperCase and StartsWith in both PDBs; retail 0x0083EBD0 (tolower 0x00BFED10) is followed by
//   its byte twin 0x0083ECF0 (toupper 0x00BFED16) and by 0x0083EE10, which calls 0x0083EBD0 twice and is the landed StartsWith.
//   The landed GetHeaderString 0x0083FCE0 carries this exact body as a same-TU stand-in.
// Label: the catalog's "CString_ToLower" is descriptive and wrong (STLport std::string in the LHNetworkLib namespace).
// Callees: malloc 0x00BFEA0E (the 8-byte default block, then the result block), tolower 0x00BFED10, basic_string<char>::
//   reserve 0x00777DB0, memmove 0x00BFEAE6, free 0x00BFEA14, _String_base::_M_throw_length_error 0x00565A30.
// Layout: STLport 4.6 _String_base<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Flags: /Gr
// TU: /Gr (the LHNetworkLib TU: free functions are __fastcall without a keyword, as the landed StartsWith 0x0083EE10).
// Shape: the landed GetHeaderString stand-in `std::string result; int length = text.size(); for (int i = 0; i < length; ++i) result +=
//   (char)tolower(text[i]); return result;` (no NRVO: the return copy-constructs from the local, then frees it), with
//   the real STLport 4.6 default constructor (allocate(8) in the body) and same-TU out-of-line bodies of reserve and
//   _M_throw_length_error: with only declarations the local's address escapes and VC7.1 keeps it in memory (dead zero
//   stores, 0x18 frame, 313v281); with the bodies it keeps start/finish in edi/esi and syncs them around reserve.
#include <string.h>

extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6
extern "C" int __cdecl tolower(int c);                                                // 0x00BFED10
extern "C" int __cdecl puts(const char* text);                                        // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                                // 0x00BFEAEC

namespace std {

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };

template <class _CharT> struct char_traits;
template <> struct char_traits<char> {
public:
    typedef char char_type;
    static void assign(char& __c1, const char& __c2) { __c1 = __c2; }
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
    __declspec(noreturn) void _M_throw_length_error() const;                           // 0x00565A30
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
    enum { _DEFAULT_SIZE = 8 };

    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    basic_string() : _String_base<_CharT, _Alloc>(allocator_type())                   // STLport 4.6 _string.c
    {
        this->_M_start = this->_M_end_of_storage.allocate(8);
        this->_M_finish = this->_M_start;
        this->_M_end_of_storage._M_data = this->_M_start + 8;
        _M_terminate_string();
    }
    basic_string(const _Self& __s) : _String_base<_CharT, _Alloc>(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }
    ~basic_string() { }

    size_type size() const { return this->_M_finish - this->_M_start; }
    const _CharT& operator[](size_type __n) const { return *(this->_M_start + __n); }
    void reserve(size_type __res_arg = 0);                                             // 0x00777DB0 (same-TU stand-in below)
    void push_back(_CharT __c)
    {
        if (this->_M_finish + 1 == this->_M_end_of_storage._M_data)
            reserve(size() + (max)(size(), (size_type)1));
        _M_construct_null(this->_M_finish + 1);
        _Traits::assign(*(this->_M_finish), __c);
        ++this->_M_finish;
    }
    _Self& operator+=(_CharT __c) { push_back(__c); return *this; }

    template <class _ForwardIter>
    void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&)
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
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void _String_base<_Tp, _Alloc>::_M_throw_length_error() const                         // 0x00565A30 (same-TU stand-in)
{
    puts("basic_string");
    abort();
}
template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::reserve(size_type __res_arg)              // 0x00777DB0 (landed body)
{
    if (__res_arg > this->max_size())
        this->_M_throw_length_error();
    size_type __n = (max)(__res_arg, size()) + 1;
    _CharT* __new_start = this->_M_end_of_storage.allocate(__n);
    _CharT* __new_finish = __new_start;
    __new_finish = uninitialized_copy(this->_M_start, this->_M_finish, __new_start);
    _M_construct_null(__new_finish);
    this->_M_deallocate_block();
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __n;
}
#pragma auto_inline(on)

typedef basic_string<char> string;
}

namespace LHNetworkLib {

std::string toLowerCase(const std::string& text)
{
    std::string result;
    int length = text.size();
    for (int i = 0; i < length; ++i)
        result += (char)tolower(text[i]);
    return result;
}

}