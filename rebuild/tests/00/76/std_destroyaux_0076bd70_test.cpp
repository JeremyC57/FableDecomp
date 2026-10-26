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
static int g_stringDtors = 0;
class CCharString { public: const char* m_pData; CCharString() : m_pData(0) { } ~CCharString() { ++g_stringDtors; } };
struct ParticleAndEndPoints { CCharString StartDummy, EndDummy; long ParticleType, Unknown0c; long PeriodicFrames; ParticleAndEndPoints() : ParticleType(0), Unknown0c(0), PeriodicFrames(0) { } };
typedef CRetailVector<ParticleAndEndPoints> TElem;
static void DestroyAux(TElem* first, TElem* last, const __false_type&)
{
    for (; first != last; ++first)
        first->~TElem();
}

int main()
{
    static double storage[3 * sizeof(TElem) / sizeof(double) + 2];
    static double innerStorage[6 * sizeof(ParticleAndEndPoints) / sizeof(double) + 2];
    TElem* elems = (TElem*)storage;
    ParticleAndEndPoints* inner = (ParticleAndEndPoints*)innerStorage;
    int i, used = 0;
    for (i = 0; i < 6; ++i) new ((void*)(inner + i)) ParticleAndEndPoints();
    for (i = 0; i < 3; ++i) { new ((void*)(elems + i)) TElem(); elems[i].Attach(inner + used, i + 1); used += i + 1; }
    DestroyAux(elems, elems + 3, __false_type());
    bool ok = g_frees == 3 && g_lastFreed == inner + 3 && g_stringDtors == 2 * 6;
    if (!ok) { std::printf("0076bd70_TEST FAIL\n"); return 1; }
    std::printf("0076bd70_TEST PASS\n");
    return 0;
}