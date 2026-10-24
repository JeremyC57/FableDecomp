// std::operator+(const basic_string& s, const basic_string& y) -- STLport string concatenation: reserve size()+size(), append both, return the result by value
// Leaf: ??Hstd
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569690, 202 bytes, `__cdecl` returning the string through the hidden
//   pointer (`ret`). The catalog label "CTCElectrocutedEffect_InitFromStringRange" is propagated/descriptive.
// Callees: malloc @ 0x00BFEA0E, basic_string::append(const char*, const char*, const forward_iterator_tag&)
//   @ 0x00567400 (x2, landed), basic_string::_M_range_initialize(const char*, const char*, const
//   forward_iterator_tag&) @ 0x00567D00 (landed; the copy constructor of the return value), free @ 0x00BFEA14
//   (~__result), puts @ 0x00BFEAF2 / abort @ 0x00BFEAEC (the length error, inlined here).
// Layout: STLport basic_string<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x verbatim: `_Str __result(_Reserve_t(), s.size() + y.size()); __result.append(s);
//   __result.append(y); return __result;`. The _Reserve_t ctor is `_String_base(a, n + 1)` +
//   `_M_terminate_string()` (_M_allocate_block's `n <= max_size()+1 && n > 0` test -> the inlined
//   `puts("basic_string"); abort();` of _M_throw_length_error, abort noreturn as in <stdlib.h>: the cold
//   block ends `jmp abort`). VC7.1 has no NRVO: the return copies __result through the out-of-line range
//   initialise and frees it. The tag temporaries share the dead `s` argument slot.
// Levers: VC7.1 would not inline the _Reserve_t ctor (it called it with a zero-byte _Reserve_t), so it is
//   `__forceinline`; append and _M_range_initialize carry their landed bodies as same-TU
//   `__declspec(noinline)` stand-ins (only operator+ is graded) -- that is what keeps __result unaliased, so
//   the terminator store goes first, the end-of-storage store sinks to the first call and __result._M_start
//   stays in edi across the copy.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
extern "C" int   __cdecl puts(const char* text);                                 // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                                              // 0x00BFEAEC

namespace std {

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
    _String_base(const allocator_type& __a, unsigned int __n)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0)
    {
        _M_allocate_block(__n);
    }
    ~_String_base() { _M_deallocate_block(); }
};

class basic_string : public _String_base {
public:
    typedef unsigned int size_type;
    struct _Reserve_t { };

    __forceinline basic_string(_Reserve_t, unsigned int __n, const allocator_type& __a = allocator_type())
        : _String_base(__a, __n + 1)
    {
        _M_terminate_string();
    }
    basic_string(const basic_string& __s) : _String_base(__s.get_allocator())
    {
        _M_range_initialize(__s._M_start, __s._M_finish);
    }

    allocator_type get_allocator() const { return _M_end_of_storage; }
    size_type size() const { return _M_finish - _M_start; }
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }

    __declspec(noinline) void _M_range_initialize(const char* __f, const char* __l, const forward_iterator_tag&)
    {                                                                                          // 0x00567D00
        int __n = __l - __f;
        _M_allocate_block(__n + 1);
        _M_finish = uninitialized_copy(__f, __l, _M_start);
        _M_terminate_string();
    }
    void _M_range_initialize(const char* __f, const char* __l)
    {
        _M_range_initialize(__f, __l, forward_iterator_tag());
    }

    size_type capacity() const { return (_M_end_of_storage._M_data - _M_start) - 1; }
    __declspec(noinline) basic_string& append(const char* __first, const char* __last, const forward_iterator_tag&)
    {                                                                                          // 0x00567400
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
    basic_string& append(const char* __first, const char* __last)
    {
        return append(__first, __last, forward_iterator_tag());
    }
    basic_string& append(const basic_string& __s) { return append(__s._M_start, __s._M_finish); }
};

basic_string operator+(const basic_string& __s, const basic_string& __y)
{
    typedef basic_string _Str;
    typedef _Str::_Reserve_t _Reserve_t;
    _Str __result(_Reserve_t(), __s.size() + __y.size());
    __result.append(__s);
    __result.append(__y);
    return __result;
}

}