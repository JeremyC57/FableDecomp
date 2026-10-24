// std::vector<const CThingCreatureBase*>::reserve(unsigned int n) -- STLport reserve for a vector of pointers: reallocate to n slots when the capacity is smaller, keeping the elements
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CE230, length 106 bytes (+ int3 padding), thiscall `ret 4`. Single
// caller: CGroundNavigatorExternalInterface::IsMovementBlockedAt (0x006CDF5C, `reserve(8)` on its local result vector
// before GetAllThingsInRoughArea 0x006CE300 fills it; the landed model of that caller names the element
// `const CThingCreatureBase*`, the bytes only show a 4-byte POD). The manifest label "CActiveFile_Reserve" is
// propagated.
// Callees: _M_allocate_and_copy(n, first, last) @ 0x006CE2B0 (this chunk's row), _M_clear() @ 0x006CE2A0 (`if (start)
// free(start)`; landed under the propagated "CActiveFile::OnReadFinished" label), malloc @ 0x00BFEA0E (the inline
// allocator on the empty-vector path).
// Shape: STLport 4.x verbatim (speed TU, unlike the size/frame-pointer CRetailVector20::reserve 0x005C2013):
// `capacity() < n` unsigned, the old size computed first (ebx), then either _M_allocate_and_copy + _M_clear (a buffer
// exists) or the inline `allocate(n)` = `n != 0 ? malloc(n * 4) : 0`, and the inline _M_set(tmp, tmp + size, tmp + n).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct CThingCreatureBase;

namespace std {

template <class T> struct allocator {
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    typedef unsigned int size_type;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage._M_data - _M_start); }
    T* _M_allocate_and_copy(size_type n, T* first, T* last);   // 0x006CE2B0
    void _M_clear();                                            // 0x006CE2A0
    void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void reserve(size_type n);
};

template <class T, class A> void vector<T, A>::reserve(size_type n)
{
    if (capacity() < n) {
        const size_type old_size = size();
        T* tmp;
        if (_M_start) {
            tmp = _M_allocate_and_copy(n, _M_start, _M_finish);
            _M_clear();
        } else {
            tmp = _M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

template void vector<const CThingCreatureBase*>::reserve(unsigned int);
}