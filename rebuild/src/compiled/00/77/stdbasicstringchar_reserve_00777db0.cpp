// std::basic_string<char>::reserve(unsigned int res_arg) -- STLport string reserve: move the characters into a fresh max(res_arg, size()) + 1 byte block, terminate it and free the old block
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00777DB0, 140 bytes, thiscall `ret 4`.
// Callees: basic_string::_M_throw_length_error() const @ 0x00565A30 (the exceptions-off `puts("basic_string")` +
//   abort trampoline, `__declspec(noreturn)`: VC7.1 keeps the call inline before the dead fall-through, still assumes
//   res_arg in ecx after it, and pushes ebx/ebp/esi only after the check), malloc @ 0x00BFEA0E
//   (allocator<char>::allocate, null for 0), memmove @ 0x00BFEAE6 (the trivial uninitialized_copy), free @ 0x00BFEA14
//   (_M_deallocate_block).
// Callers: basic_string<char>::push_back 0x00777F80 (this chunk) and 0x00774A37, 0x0083EC5A, 0x0083ED7A.
// Label correction: catalogued as `CString_Reserve`; the body is STLport `basic_string<char>::reserve`.
// Layout: STLport _String_base<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x verbatim: `if (res_arg > max_size()) _M_throw_length_error(); n = (max)(res_arg, size()) + 1;
//   new_start = allocate(n); new_finish = uninitialized_copy(_M_start, _M_finish, new_start); _M_construct_null(
//   new_finish); _Destroy(..); _M_deallocate_block(); _M_start = new_start; _M_finish = new_finish; _M_end_of_storage =
//   new_start + n;` -- `max` by reference (the size spilled to the `push ecx` frame word, n stored back over res_arg),
//   the trivial copy is `last == first ? result : memmove(result, first, n) + n`.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                                          // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace std {

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

class basic_string {
public:
    typedef unsigned int size_type;
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;

    size_type size() const { return _M_finish - _M_start; }
    size_type max_size() const { return size_type(-1) - 1; }
    char* allocate(size_type __n) { return __n != 0 ? (char*)malloc(__n) : 0; }
    void _M_deallocate_block() { if (_M_start) free(_M_start); }
    void _M_construct_null(char* __p) { *__p = 0; }
    __declspec(noreturn) void _M_throw_length_error() const;                    // 0x00565A30
    void reserve(size_type __res_arg);
};

void basic_string::reserve(size_type __res_arg)
{
    if (__res_arg > max_size())
        _M_throw_length_error();

    size_type __n = (max)(__res_arg, size()) + 1;
    char* __new_start = allocate(__n);
    char* __new_finish = __new_start;

    __new_finish = uninitialized_copy(_M_start, _M_finish, __new_start);
    _M_construct_null(__new_finish);

    _M_deallocate_block();
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __n;
}

}