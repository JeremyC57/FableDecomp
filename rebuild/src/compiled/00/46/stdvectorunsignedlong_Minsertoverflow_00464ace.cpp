// std::vector<unsigned long>::_M_insert_overflow(unsigned long* pos, const unsigned long& x, const __true_type&, unsigned int fill_len, bool atend) -- STLport POD growth insert: allocate old + max(old, fill_len) words, copy the head, write fill_len copies of x, copy the tail unless atend, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00464ACE, length 188 bytes, `ret 14h` (thiscall: five arguments).
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6 (__copy_trivial), free @ 0x00BFEA14 (_M_clear).
// Label: the catalog "Vector_InsertN_uint32" is behavioural; 44 callers, among them the landed push_back bodies 0x005178A0 /
// 0x009D0630 (which call it as `(last, value, tag, 1, true)`).
// Model: STLport 4.x `_M_insert_overflow(pos, x, const __true_type&, fill_len, atend)` verbatim: `__len = __old_size +
// (max)(__old_size, __fill_len)` through the const-reference `max` (`lea ecx,[ebp+14h]` / `lea ecx,[ebp-4]`), allocate,
// `__copy_trivial(start, pos, new_start)`, `fill_n(new_finish, fill_len, x)` (x re-read every pass), the tail copy when
// !atend, the inline `_M_clear()` and `_M_set(new_start, new_finish, new_start + len)`.
// Shape: frame pointer, `and dword ptr [ebp-4],0` = size TU with /Oy off: #pragma optimize("y", off) + ("s", on).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);                                    // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

namespace std {
struct __true_type { };

template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

__forceinline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}

template <class OutputIter, class Size, class T>
__forceinline OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    __forceinline void deallocate(T* p, unsigned int) { free(p); }
};

template <class T, class A = allocator<T> > class vector {
public:
    typedef T* pointer;
    pointer _M_start;
    pointer _M_finish;
    struct : A { pointer _M_data; } _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    __forceinline void _M_clear()
    {
        if (_M_start)
            _M_end_of_storage.deallocate(_M_start, (unsigned int)(_M_end_of_storage._M_data - _M_start));
    }
    __forceinline void _M_set(pointer s, pointer f, pointer e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage._M_data = e;
    }
    void _M_insert_overflow(pointer pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);
};

template <class T, class A>
void vector<T, A>::_M_insert_overflow(pointer pos, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    pointer new_start = _M_end_of_storage.allocate(len);
    pointer new_finish = (pointer)__copy_trivial(_M_start, pos, new_start);
    new_finish = fill_n(new_finish, fill_len, x);
    if (!atend)
        new_finish = (pointer)__copy_trivial(pos, _M_finish, new_finish);
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<unsigned long>;
}