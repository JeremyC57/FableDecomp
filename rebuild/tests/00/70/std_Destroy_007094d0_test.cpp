#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_frees = 0;
static void* g_lastFreed = 0;
static void TestFree(void* p) { ++g_frees; g_lastFreed = p; }

struct SValueVector {
    unsigned long* _M_start; unsigned long* _M_finish; unsigned long* _M_end_of_storage;
    SValueVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~SValueVector() { if (_M_start) TestFree(_M_start); }      // trivially destructible values: only the block
};
typedef SValueVector TElem;
static void DestroyAux(TElem* first, TElem* last, const __false_type&)
{
    for (; first != last; ++first)
        first->~TElem();
}
static void Destroy(TElem* first, TElem* last)
{
    DestroyAux(first, last, __false_type());
}

int main()
{
    static double storage[3 * sizeof(TElem) / sizeof(double) + 2];
    TElem* elems = (TElem*)storage;
    int i;
    static unsigned long blocks[3][4];
    for (i = 0; i < 3; ++i) { new ((void*)(elems + i)) TElem(); if (i != 1) { elems[i]._M_start = blocks[i]; elems[i]._M_finish = blocks[i] + 2; elems[i]._M_end_of_storage = blocks[i] + 4; } }
    Destroy(elems, elems + 3);
    bool ok = g_frees == 2 && g_lastFreed == blocks[2];
    if (!ok) { std::printf("007094d0_TEST FAIL\n"); return 1; }
    std::printf("007094d0_TEST PASS\n");
    return 0;
}