// std::basic_string<unsigned short>::reserve(size_type res_arg) -- move the wide characters into a fresh max(res_arg, size()) + 1 element block, terminate it and free the old block
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099C990, 152 bytes, thiscall `ret 4`.
// Callees: basic_string<unsigned short>::_M_throw_length_error() const @ 0x0099C550 (exceptions-off puts + abort,
// `__declspec(noreturn)`: VC7.1 keeps the call inline before the fall-through), malloc @ 0x00BFEA0E
// (allocator::allocate, null for 0), memmove @ 0x00BFEAE6 (the trivial uninitialized_copy), free @ 0x00BFEA14
// (_M_deallocate_block).
// Callers: append(n, c) 0x0099CB00, push_back 0x0099CC80, CWideString::operator+=(wchar_t) 0x0099B9A0 (landed).
// Label correction: the manifest's "String_ResizeBuffer" is propagated; the body is STLport
// basic_string<unsigned short>::reserve, the wide sibling of the landed basic_string<char>::reserve 0x00777DB0.
// Shape: STLport 4.x verbatim -- `max` by reference (size() spilled to the `push ecx` frame word, n stored back over
// res_arg), the trivial copy is `last == first ? result : memmove(result, first, bytes) + bytes`.
// Leaf: ?reserve@?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@
namespace std {

typedef unsigned int size_t;

extern "C" void* __cdecl malloc(size_t size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                                    // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);    // 0x00BFEAE6

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template<class _CharT> class char_traits { };

template<class _Tp> class allocator {
public:
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template<class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {
public:
    typedef _CharT* pointer;
    typedef size_t  size_type;

    pointer _M_start;            // +0x00
    pointer _M_finish;           // +0x04
    pointer _M_end_of_storage;   // +0x08

    size_type size() const { return _M_finish - _M_start; }
    size_type max_size() const { return size_type(-1) / sizeof(_CharT) - 1; }
    __declspec(noreturn) void _M_throw_length_error() const;   // 0x0099C550
    void _M_deallocate_block() { if (_M_start) free(_M_start); }
    void _M_construct_null(pointer __p) { *__p = 0; }

    void reserve(size_type __res_arg);
};

template<class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::reserve(size_type __res_arg)
{
    if (__res_arg > max_size())
        _M_throw_length_error();

    size_type __n = (max)(__res_arg, size()) + 1;
    _Alloc __a;
    pointer __new_start = __a.allocate(__n);
    pointer __new_finish = __new_start;

    __new_finish = (pointer)__copy_trivial(_M_start, _M_finish, __new_start);
    _M_construct_null(__new_finish);

    _M_deallocate_block();
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __n;
}

template void basic_string<unsigned short>::reserve(size_type);

}