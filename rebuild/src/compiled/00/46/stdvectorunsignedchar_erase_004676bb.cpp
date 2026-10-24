// std::vector<unsigned char>::erase(unsigned char* first, unsigned char* last) -- STLport range erase of a byte vector: slide the tail down with memmove, move _M_finish, return first
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) @ 0x004676BB, 52 bytes, `ret 8` (__fastcall: the vector in ecx, first/last on the stack).
// Callees: memmove @ 0x00BFEAE6 (__cdecl, `add esp,0Ch`). Caller: vector<unsigned char>::resize @ 0x00467686
//   (`if (n < size()) erase(begin() + n, end()); else insert(...)`), itself reached from CPersistContext::Transfer<signed char>.
// Shape: STLport 4.x `erase(first, last) { pointer i = __copy_ptrs(last, _M_finish, first, _TrivialAss()); _Destroy(i, _M_finish);
//   _M_finish = i; return first; }` -- for a POD byte the copy is `__copy_trivial`, the `(last == first) ? result :
//   (char*)memmove(result, first, n) + n` helper (retail compares _M_finish with `last`, then reuses the difference for
//   the add), and _Destroy of a trivial type vanishes. Frame-pointer size TU: `optimize("y", off)` + `("s", on)` gives the
//   `push dword ptr [ebp+N]` argument re-reads with no spills; the helper must be `__forceinline` (the size TU calls a plain
//   `inline` one out of line).
// Model: the three STLport vector words (_M_start, _M_finish, _M_end_of_storage); no engine header (library type).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6

namespace std {
__forceinline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}
template <class T> class vector {
public:
    T* _M_start;                    // +0x00
    T* _M_finish;                   // +0x04
    T* _M_end_of_storage;           // +0x08
    T* erase(T* first, T* last);
};
template <class T> T* vector<T>::erase(T* first, T* last)
{
    T* i = (T*)__copy_trivial(last, _M_finish, first);
    _M_finish = i;
    return first;
}
template unsigned char* vector<unsigned char>::erase(unsigned char* first, unsigned char* last);
}