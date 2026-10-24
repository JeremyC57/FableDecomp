// std::vector<CCharString>::reserve(unsigned int n) -- STLport: grow the storage to n slots when the capacity is smaller, moving the live strings by copy-and-clear
// Leaf: reserve
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BA7A0, length 104 bytes, `ret 4` (thiscall: n).
// Callees: vector<CCharString>::_M_allocate_and_copy @ 0x004B83E0 (this chunk), vector<CCharString>::_M_clear @ 0x00433040
// (the landed CCharStringVector::operator= 0x00432EE9 calls the same pair), malloc @ 0x00BFEA0E.
// Label: the catalog "CCombatWheel_Resize" is propagated; the body is STLport 4.x `vector::reserve(size_type)`:
// `if (capacity() < n) { old = size(); tmp = start ? _M_allocate_and_copy(n, start, finish) + _M_clear() : allocate(n);
// _M_set(tmp, tmp + old, tmp + n); }`. Ten callers (0x004B2D07 .., 0x00A6D074 ..) reserve CCharString name lists.
// Shape: default /O2 TU; the empty-vector branch allocates inline (`lea edx,[ebp*4] ; call malloc`, zero for n == 0).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct CCharStringData;
class CCharString {
private:
    CCharStringData* m_pData;
};

namespace std {
template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    __forceinline unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    __forceinline unsigned int capacity() const { return unsigned int(_M_end_of_storage._M_data - _M_start); }
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);   // 0x004B83E0
    void _M_clear();                                                          // 0x00433040
    void reserve(unsigned int n);
};

template <class T, class A> void vector<T, A>::reserve(unsigned int n)
{
    if (capacity() < n) {
        const unsigned int old_size = size();
        T* tmp;
        if (_M_start) {
            tmp = _M_allocate_and_copy(n, _M_start, _M_finish);
            _M_clear();
        }
        else {
            tmp = _M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

template void vector<CCharString, allocator<CCharString> >::reserve(unsigned int);
}