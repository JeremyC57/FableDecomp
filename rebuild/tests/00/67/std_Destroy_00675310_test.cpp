#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_frees = 0;
static void* g_lastFreed = 0;
static void TestFree(void* p) { ++g_frees; g_lastFreed = p; }
template <class T>
inline void DestroyRange(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T>
struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void Attach(T* first, int n) { _M_start = first; _M_finish = first + n; _M_end_of_storage = first + n; }
    ~CRetailVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            TestFree(_M_start);
    }
};
struct CTextureMorph { unsigned long NewTexture; unsigned long GroupID; };
struct STextureMorphEntry { unsigned long first; CTextureMorph second; };
struct CKeyPairCompareLess { };
struct CTextureMorphMap { CRetailVector<STextureMorphEntry> m_vector; CKeyPairCompareLess Compare; bool Dirty; CTextureMorphMap() : Dirty(false) { } };
typedef CTextureMorphMap TElem;
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
    static STextureMorphEntry blocks[3][2];
    for (i = 0; i < 3; ++i) { new ((void*)(elems + i)) TElem(); if (i != 0) elems[i].m_vector.Attach(blocks[i], 2); }
    Destroy(elems, elems + 3);
    bool ok = g_frees == 2 && g_lastFreed == blocks[2];
    if (!ok) { std::printf("00675310_TEST FAIL\n"); return 1; }
    std::printf("00675310_TEST PASS\n");
    return 0;
}