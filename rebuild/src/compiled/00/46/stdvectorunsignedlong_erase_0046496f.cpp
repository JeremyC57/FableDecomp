// std::vector<unsigned long>::erase(unsigned long* first, unsigned long* last) -- STLport range erase of a POD vector: memmove the tail [last, finish) down onto first, finish = first + tail, return first
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046496F, length 52 bytes, `ret 8`.
// Callees: memmove @ 0x00BFEAE6.
// Label: the catalog "std_vector_push_copy_element" is wrong: the landed CQBanditCampScript::CheckAnyBanditsKilled
// (0x00D032F0) calls this body as vector<unsigned long>::erase(first, last) (its clear()); seven more callers.
// Model: STLport 4.x `erase(first, last)`: `__copy_ptrs(last, finish, first, _TrivialAss())` = __copy_trivial in its
// `(last == first) ? result : memmove(...) + n` form (inlined), the trivial _Destroy, `_M_finish = i`, `return first`.
// Shape: frame pointer with `push dword ptr [ebp+N]` pass-through arguments = size TU with /Oy off:
// #pragma optimize("y", off) + ("s", on).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

namespace std {
struct __true_type { };

__forceinline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}

template <class T> __forceinline T* __copy_ptrs(const T* first, const T* last, T* result, const __true_type&)
{
    return (T*)__copy_trivial(first, last, result);
}

template <class T> __forceinline void _Destroy(T*, T*) { }

template <class T> class vector {
public:
    typedef T* pointer;
    typedef T* iterator;
    typedef __true_type _TrivialAss;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;
    iterator erase(iterator first, iterator last);
};

template <class T> typename vector<T>::iterator vector<T>::erase(iterator first, iterator last)
{
    pointer i = __copy_ptrs(last, _M_finish, first, _TrivialAss());
    _Destroy(i, _M_finish);
    _M_finish = i;
    return first;
}

template class vector<unsigned long>;
}