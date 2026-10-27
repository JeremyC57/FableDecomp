// Behaviour test for 0x00A6BE40 CArray<CIVCountedPointer<CManagedTextureData> >::DeallocateMemory()
// Model: clear() destroys every element, then swapping with an empty temporary frees the storage (capacity 0 after).
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_destroyed = 0;
class CPtr {
public:
    int* Object;
    ~CPtr() { ++g_destroyed; }
};
class TestArray {
public:
    CPtr* _M_start;
    CPtr* _M_finish;
    CPtr* _M_end_of_storage;
    TestArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~TestArray() { clear(); if (_M_start) std::free(_M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    void clear() { for (CPtr* p = _M_start; p != _M_finish; ++p) p->~CPtr(); _M_finish = _M_start; }
    void swap(TestArray& x)
    {
        CPtr* t;
        t = _M_start; _M_start = x._M_start; x._M_start = t;
        t = _M_finish; _M_finish = x._M_finish; x._M_finish = t;
        t = _M_end_of_storage; _M_end_of_storage = x._M_end_of_storage; x._M_end_of_storage = t;
    }
    void DeallocateMemory()
    {
        if (capacity() > 0) {
            clear();
            TestArray().swap(*this);
        }
    }
};
int main()
{
    TestArray a;
    a.DeallocateMemory();                                  // empty: nothing happens
    if (a.capacity() != 0 || g_destroyed != 0) { std::printf("00a6be40_TEST FAIL\n"); return 1; }
    a._M_start = (CPtr*)std::malloc(4 * sizeof(CPtr));
    for (int i = 0; i < 3; ++i) new (a._M_start + i) CPtr();
    a._M_finish = a._M_start + 3;
    a._M_end_of_storage = a._M_start + 4;
    a.DeallocateMemory();
    if (g_destroyed != 3 || a._M_start != 0 || a._M_finish != 0 || a._M_end_of_storage != 0) { std::printf("00a6be40_TEST FAIL\n"); return 1; }
    std::printf("00a6be40_TEST PASS\n");
    return 0;
}