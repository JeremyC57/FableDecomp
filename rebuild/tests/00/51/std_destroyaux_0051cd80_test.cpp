#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_frees = 0;
static void* g_lastFreed = 0;
static void TestFree(void* p) { ++g_frees; g_lastFreed = p; }
struct C2DLineF { float X1, Y1, X2, Y2; };
struct CSwitchableBlockage { unsigned long UniqueIDLow, UniqueIDHigh; void* LineList; };
static int g_blockageRanges = 0, g_blockagesDestroyed = 0;
static void DestroyBlockages(CSwitchableBlockage* first, CSwitchableBlockage* last, const __false_type&)   // 0x00519EF0 stand-in
{
    ++g_blockageRanges;
    g_blockagesDestroyed += (int)(last - first);
}
inline void DestroyElements(C2DLineF*, C2DLineF*) { }
inline void DestroyElements(CSwitchableBlockage* first, CSwitchableBlockage* last) { DestroyBlockages(first, last, __false_type()); }
template <class T>
struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void Attach(T* first, int n) { _M_start = first; _M_finish = first + n; _M_end_of_storage = first + n; }
    ~CRetailVector() { DestroyElements(_M_start, _M_finish); if (_M_start) TestFree(_M_start); }
};
struct CNavigationLayerUpdate { CRetailVector<C2DLineF> LineList; CRetailVector<CSwitchableBlockage> SwitchableBlockages; };
typedef CNavigationLayerUpdate TElem;
static C2DLineF g_lines[3][2];
static CSwitchableBlockage g_blockages[3][3];
static void DestroyAux(TElem* first, TElem* last, const __false_type&)
{
    for (; first != last; ++first)
        first->~TElem();
}

int main()
{
    static double storage[3 * sizeof(TElem) / sizeof(double) + 2];
    TElem* elems = (TElem*)storage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(elems + i)) TElem(); elems[i].LineList.Attach(g_lines[i], 2); if (i != 0) elems[i].SwitchableBlockages.Attach(g_blockages[i], i + 1); }
    DestroyAux(elems, elems + 3, __false_type());
    bool ok = g_frees == 3 + 2 && g_lastFreed == g_lines[2] && g_blockageRanges == 3 && g_blockagesDestroyed == 2 + 3;
    if (!ok) { std::printf("0051cd80_TEST FAIL\n"); return 1; }
    std::printf("0051cd80_TEST PASS\n");
    return 0;
}