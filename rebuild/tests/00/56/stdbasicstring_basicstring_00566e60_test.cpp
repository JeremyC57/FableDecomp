// Behaviour test for 0x00566E60 std::basic_string::basic_string(): an empty STLport string owns an 8-byte block
// whose first byte is the terminator.
#include <cstdio>
#include <cstdlib>

namespace stl {
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
class _String_base {
public:
    typedef allocator<char> allocator_type;
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
};
class basic_string : public _String_base {
public:
    basic_string();
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }
};
basic_string::basic_string() : _String_base(allocator_type())
{
    _M_start = _M_end_of_storage.allocate(8);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + 8;
    _M_terminate_string();
}
}

int main()
{
    stl::basic_string s;
    if (s._M_start == 0 || s._M_finish != s._M_start || s._M_end_of_storage._M_data != s._M_start + 8
        || s._M_start[0] != 0) {
        std::printf("00566e60_TEST FAIL\n");
        return 1;
    }
    s._M_start[7] = 'x';                       // the whole 8-byte block is writable
    std::free(s._M_start);
    std::printf("00566e60_TEST PASS\n");
    return 0;
}