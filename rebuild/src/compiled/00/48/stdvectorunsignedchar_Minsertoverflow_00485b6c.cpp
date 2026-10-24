// std::vector<unsigned char>::_M_insert_overflow(unsigned char* position, const unsigned char& x, const __true_type&, size_t fill_len, bool atend) -- STLport POD grow-and-insert: allocate size + max(size, n), copy the head, fill n bytes of x, copy the tail, free the old block
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485B6C, 132 bytes, `ret 14h`.
// Callees: malloc @ 0x00BFEA0E, __copy_trivial(first, last, result) @ 0x00405BA0 (`__fastcall` /Gr, memmove),
// fill(unsigned char*, unsigned char*, const unsigned char&) @ 0x0047CE40 (row of this chunk), free @ 0x00BFEA14.
// Shape: STLport 4.x `_M_insert_overflow` for a POD element, verbatim: `__len = __old_size + (max)(__old_size,
// __fill_len)` (const-reference max: the old size goes to the frame word for its address), allocate,
// `__new_finish = __copy_trivial(_M_start, position, new_start)`, `__new_finish = fill_n(new_finish, fill_len, x)`
// (the inline `unsigned char` overload: `fill(first, first + n, x); return first + n;`), the tail copy unless
// `atend`, the inline `_M_clear()` (trivial destroy + `if (p) free(p)`) and `_M_set`. VC7.1 homes new_start in
// the dead `fill_len` slot (`mov [ebp+14h],eax` / `and dword ptr [ebp+14h],0`). Frame-pointer size TU
// (`optimize("y", off)` + `("s", on)`), the inline helpers `__forceinline`.
// Label: the catalog's "vector_reallocate_with_padding"; callers are the landed _M_fill_insert 0x00485F3A (as
// its documented callee) and the push_back/insert paths at 0x009E98A9.. / 0x00A4F4C4.
// Leaf: _M_insert_overflow
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

namespace std {

struct __true_type { __true_type() { } };

void* __fastcall __copy_trivial(const void* __first, const void* __last, void* __result);       // 0x00405BA0
void __fastcall fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val); // 0x0047CE40

__forceinline unsigned char* fill_n(unsigned char* __first, unsigned int __n, const unsigned char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    __forceinline void _M_clear()
    {
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    __forceinline void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                            unsigned int __fill_len, bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                             unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = _M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, _M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<unsigned char>;

}