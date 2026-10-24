// Behaviour test for 0x00568E70 std::__uninitialized_copy over STLport strings.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

namespace stl {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
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
        ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
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
    void _M_throw_length_error() const { std::puts("basic_string"); std::abort(); }
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
};
class basic_string : public _String_base {
public:
    allocator_type get_allocator() const { return _M_end_of_storage; }
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }
    void _M_range_initialize(const char* __f, const char* __l)
    {
        int __n = (int)(__l - __f);
        _M_allocate_block(__n + 1);
        _M_finish = uninitialized_copy(__f, __l, _M_start);
        _M_terminate_string();
    }
    basic_string(const basic_string& __s) : _String_base(__s.get_allocator()) { _M_range_initialize(__s._M_start, __s._M_finish); }
    basic_string(const char* s) : _String_base(allocator_type()) { _M_range_initialize(s, s + std::strlen(s)); }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

int main()
{
    stl::basic_string src[3] = { stl::basic_string("alpha"), stl::basic_string(""), stl::basic_string("gamma ray") };
    stl::basic_string* dst = (stl::basic_string*)std::malloc(sizeof(stl::basic_string) * 3);
    stl::basic_string* end = stl::__uninitialized_copy((const stl::basic_string*)src, (const stl::basic_string*)src + 3, dst,
                                                        stl::__false_type());
    int ok = end == dst + 3;
    for (int i = 0; ok && i < 3; ++i) {
        unsigned int n = (unsigned int)(src[i]._M_finish - src[i]._M_start);
        if (dst[i]._M_start == src[i]._M_start || (unsigned int)(dst[i]._M_finish - dst[i]._M_start) != n
            || dst[i]._M_end_of_storage._M_data != dst[i]._M_start + n + 1 || std::memcmp(dst[i]._M_start, src[i]._M_start, n) != 0
            || dst[i]._M_finish[0] != 0) ok = 0;
    }
    if (stl::__uninitialized_copy((const stl::basic_string*)src, (const stl::basic_string*)src, dst, stl::__false_type()) != dst) ok = 0;
    for (int i = 0; i < 3; ++i) { std::free(dst[i]._M_start); std::free(src[i]._M_start); }
    std::free(dst);
    if (!ok) { std::printf("00568e70_TEST FAIL\n"); return 1; }
    std::printf("00568e70_TEST PASS\n");
    return 0;
}