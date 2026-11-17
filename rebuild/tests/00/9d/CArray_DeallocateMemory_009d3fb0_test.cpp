#include <cstdio>
#include <cstdlib>
// Behaviour test for CArray<CCharString>::DeallocateMemory() @ 0x009D3FB0 (NF48: restated; the row was labelled ~CArray before):
// with capacity, the elements are erased, the block is detached (the array left empty with no capacity), the detached strings are
// destroyed and the block freed; with no capacity nothing happens.
static int g_erase = 0, g_dtor = 0;
class CCharString { public: void* PStringData; ~CCharString() { ++g_dtor; } };
template<class T> class CArray { public:
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void clear() { if (first != last) { ++g_erase; for (T* p = first; p != last; ++p) p->~T(); last = first; } }
    void swap(CArray& o) { T* t = first; first = o.first; o.first = t; t = last; last = o.last; o.last = t; t = end_of_storage; end_of_storage = o.end_of_storage; o.end_of_storage = t; }
    CArray() : first(0), last(0), end_of_storage(0) { }
    ~CArray() { for (T* p = first; p != last; ++p) p->~T(); if (first) std::free(first); }
    void DeallocateMemory() { if (capacity() > 0) { clear(); CArray<T> empty; swap(empty); } }
};
int main()
{
    CArray<CCharString> a; a.first = (CCharString*)std::malloc(4 * sizeof(CCharString)); a.last = a.first + 3; a.end_of_storage = a.first + 4;
    a.DeallocateMemory();
    if (g_erase != 1 || g_dtor != 3 || a.first != 0 || a.last != 0 || a.end_of_storage != 0) { std::printf("009d3fb0_TEST FAIL\n"); return 1; }
    a.DeallocateMemory();
    if (g_erase != 1 || g_dtor != 3) { std::printf("009d3fb0_TEST FAIL\n"); return 1; }
    std::printf("009d3fb0_TEST PASS\n");
    return 0;
}
