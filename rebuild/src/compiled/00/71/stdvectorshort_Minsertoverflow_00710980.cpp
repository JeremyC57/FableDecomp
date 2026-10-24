// std::vector<short>::_M_insert_overflow(pointer pos, const short& x, const __true_type&, size_type fill_len, bool atend) -- STLport grow-and-insert of a trivially copyable vector
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710980, length 209 bytes (+ int3 padding). __fastcall, `ret 14h`.
// Callees: malloc / free / memmove (CRT import thunks) @ 0x00BFEA0E / 0x00BFEA14 / 0x00BFEAE6.
// Identity: the manifest label "Vector_Reallocate_uint16" is descriptive. 2-byte elements (`sar eax,1`, word stores):
//   CAnimSet::Anims (vector<short>); callers are the inline push_backs of CAnimSet::AddAnim 0x0070D2F0 (call 0x0070D324),
//   0x0070D204 and 0x00710EFC in the CAnimateThing TU.
// Shape: STLport 4.x `_M_insert_overflow(pos, x, __true_type, fill_len, atend)` verbatim: `(max)(__old_size, __fill_len)`
//   by const reference (the old size in the `push ecx` word, `lea ecx,[..] ; jb`), `allocate(len)` = `len != 0 ?
//   malloc(len * 2) : 0` (the zero arm stores and reloads the dead fill_len slot that holds new_start), the head and tail
//   through `__copy_trivial` (memmove unless empty), the fill through `fill_n` (unsigned `jbe`), then the inline
//   `_M_clear()` (`if (p != 0) free(p)`) and `_M_set(new_start, new_finish, new_start + len)`.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)
extern "C" void* __cdecl memmove(void* destination, const void* source, unsigned int size);   // 0x00BFEAE6 (import thunk)
struct __true_type { __true_type() {} };

namespace std {
template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
          + ((const char*)__last - (const char*)__first);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {   // the end-of-storage pointer
public:
    _Value _M_data;
};

template <class _Tp> class vector {                     // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = _M_end_of_storage.allocate(__len);
    _Tp* __new_finish = (_Tp*)__copy_trivial(_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (_Tp*)__copy_trivial(__position, _M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<short>;
}