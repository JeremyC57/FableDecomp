// LHNetworkLib::LHHttpHeaders::AddHeader(const string& name, const string& value) -- appends one "name: value" header line (a ':' added unless the name already ends in one) to the header list
// Leaf: ?AddHeader@LHHttpHeaders@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FC00, 219 bytes, __fastcall, `ret 8`, no EH frame (the LHNetworkLib TU).
// Identity: Ego_r.pdb ?AddHeader@LHHttpHeaders@LHNetworkLib@@QAEXABV?$basic_string@...@std@@0@Z (QAEX: void, two
//   const string&). Next to LHHttpHeaders::Reset (landed 0x0083FBC0). The manifest label "HTTP_Request_BuildPath" is
//   an autoname.
// Callees: STLport basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&)
//   0x00567D00 (the copy constructor, twice), append<const char*>(f, l, const forward_iterator_tag&) 0x00567D60 (":" at
//   0x0124C528, " " at 0x01258C1C), append<char*> 0x00567400 (+= value), vector<string>::_M_insert_overflow(pos, x,
//   const __false_type&, 1, true) 0x00569E30, free 0x00BFEA14 (~header).
// Layout: LHHttpHeaders {vector<string> HdrDocumentHeader +0, string HeaderString +0xc} (landed Reset 0x0083FBC0).
// Shape: `string header(name); if (name[name.size() - 1] != ':') header += ":"; header += " "; header += value;
//   HdrDocumentHeader.push_back(header);` -- STLport push_back inline (placement null test, zeroed _String_base, range
//   initialise out of line, else _M_insert_overflow); every tag temporary homed in the dead `name` argument slot.
//   Retail keeps `header` unaliased: header._M_start is loaded once into edi before the push_back branch and reused by
//   the copy and the destructor across both calls, and the copy's source loads are hoisted above the placement zero
//   stores. That needs every callee that receives &header to be a same-TU noinline body that never stores the address
//   (GOTCHAS "retail keeps a local unaliased"): _M_range_initialize, the two append instantiations written as
//   non-template overloads, _M_throw_length_error with its puts/abort body, and _M_insert_overflow with its two
//   uninitialized helpers, defined out of class and explicitly instantiated above the caller. Declared-only callees, or
//   template stand-ins instantiated after the caller, reload header._M_start (223 v 219); /Oa over-caches name too.
#include <string.h>
#pragma intrinsic(strlen)
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
extern "C" int   __cdecl puts(const char* text);                                 // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                           // 0x00BFEAEC

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

class _String_base {
public:
    typedef allocator<char> allocator_type;
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;

    unsigned int max_size() const { return (unsigned int)(-1) / sizeof(char) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        } else
            _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_throw_length_error() const { puts("basic_string"); abort(); }      // (0x00565A30 out of line elsewhere)
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
    ~_String_base() { _M_deallocate_block(); }
};

class basic_string : public _String_base {
public:
    typedef unsigned int size_type;
    allocator_type get_allocator() const { return _M_end_of_storage; }
    size_type size() const { return _M_finish - _M_start; }
    size_type capacity() const { return (_M_end_of_storage._M_data - _M_start) - 1; }
    const char& operator[](size_type __n) const { return *(_M_start + __n); }
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }

    __declspec(noinline) void _M_range_initialize(const char* __f, const char* __l, const forward_iterator_tag&)
    {                                                                                          // 0x00567D00
        int __n = __l - __f;
        _M_allocate_block(__n + 1);
        _M_finish = uninitialized_copy(__f, __l, _M_start);
        _M_terminate_string();
    }
    void _M_range_initialize(const char* __f, const char* __l) { _M_range_initialize(__f, __l, forward_iterator_tag()); }
    basic_string(const basic_string& __s) : _String_base(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }

    __declspec(noinline) basic_string& append(char* __first, char* __last, const forward_iterator_tag&)
    {                                                         // append<char*> 0x00567400
        if (__first != __last) {
            const size_type __old_size = size();
            int __n = __last - __first;
            if ((size_type)__n > max_size() || __old_size > max_size() - (size_type)__n)
                _M_throw_length_error();
            if (__old_size + __n > capacity()) {
                const size_type __len = __old_size + (__old_size < (size_type)__n ? (size_type)__n : __old_size) + 1;
                char* __new_start = _M_end_of_storage.allocate(__len);
                char* __new_finish = __new_start;
                __new_finish = uninitialized_copy(_M_start, _M_finish, __new_start);
                __new_finish = uninitialized_copy(__first, __last, __new_finish);
                _M_construct_null(__new_finish);
                _M_deallocate_block();
                _M_start = __new_start;
                _M_finish = __new_finish;
                _M_end_of_storage._M_data = __new_start + __len;
            }
            else {
                char* __f1 = __first;
                ++__f1;
                uninitialized_copy(__f1, __last, _M_finish + 1);
                _M_construct_null(_M_finish + __n);
                *_M_finish = *__first;
                _M_finish += __n;
            }
        }
        return *this;
    }
    __declspec(noinline) basic_string& append(const char* __first, const char* __last, const forward_iterator_tag&)
    {                                                         // append<const char*> 0x00567D60
        if (__first != __last) {
            const size_type __old_size = size();
            int __n = __last - __first;
            if ((size_type)__n > max_size() || __old_size > max_size() - (size_type)__n)
                _M_throw_length_error();
            if (__old_size + __n > capacity()) {
                const size_type __len = __old_size + (__old_size < (size_type)__n ? (size_type)__n : __old_size) + 1;
                char* __new_start = _M_end_of_storage.allocate(__len);
                char* __new_finish = __new_start;
                __new_finish = uninitialized_copy(_M_start, _M_finish, __new_start);
                __new_finish = uninitialized_copy(__first, __last, __new_finish);
                _M_construct_null(__new_finish);
                _M_deallocate_block();
                _M_start = __new_start;
                _M_finish = __new_finish;
                _M_end_of_storage._M_data = __new_start + __len;
            }
            else {
                const char* __f1 = __first;
                ++__f1;
                uninitialized_copy(__f1, __last, _M_finish + 1);
                _M_construct_null(_M_finish + __n);
                *_M_finish = *__first;
                _M_finish += __n;
            }
        }
        return *this;
    }
    basic_string& append(char* __first, char* __last) { return append(__first, __last, forward_iterator_tag()); }
    basic_string& append(const char* __first, const char* __last)
    {
        return append(__first, __last, forward_iterator_tag());
    }
    basic_string& append(const basic_string& __s) { return append(__s._M_start, __s._M_finish); }
    basic_string& append(const char* __s) { return append(__s, __s + strlen(__s)); }
    basic_string& operator+=(const basic_string& __s) { return append(__s); }
    basic_string& operator+=(const char* __s) { return append(__s); }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00568E70
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00568F10
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear();                                                                           // 0x00569A40
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x00569E30
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Copy_Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                                  size_type __fill_len, bool __atend)
{                                                                                              // 0x00569E30
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template basic_string* __fastcall __uninitialized_copy(basic_string*, basic_string*, basic_string*, const __false_type&);
template basic_string* __fastcall __uninitialized_fill_n(basic_string*, unsigned int, const basic_string&,
                                                         const __false_type&);
template void vector<basic_string>::_M_insert_overflow(basic_string*, const basic_string&, const __false_type&,
                                                       unsigned int, bool);

}

typedef std::basic_string string;

namespace LHNetworkLib {
class LHHttpHeaders {
public:
    std::vector<string> HdrDocumentHeader;         // +0x00
    string              HeaderString;              // +0x0c
    void AddHeader(const string& name, const string& value);
};

void LHHttpHeaders::AddHeader(const string& name, const string& value)
{
    string header(name);
    if (name[name.size() - 1] != ':')
        header += ":";
    header += " ";
    header += value;
    HdrDocumentHeader.push_back(header);
}
}