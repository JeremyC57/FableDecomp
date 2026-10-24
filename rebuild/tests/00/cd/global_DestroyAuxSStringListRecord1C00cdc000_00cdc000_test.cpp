#include <cstdio>
#include <cstdlib>
#include <new>

static int g_frees = 0;
static int g_stringsDestroyed = 0;
static void CountingFree(void* p) { if (p) ++g_frees; std::free(p); }

struct CCharString {
    void* m_pData;
    ~CCharString() { ++g_stringsDestroyed; m_pData = 0; }
};

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
    ~CRetailVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            CountingFree(_M_start);
    }
};

template <class T>
static void MakeVector(CRetailVector<T>& v, int n)
{
    v._M_start = n ? (T*)std::malloc(sizeof(T) * n) : 0;
    for (int i = 0; i < n; ++i) new (&v._M_start[i]) T();
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_finish;
}

struct SFalseType { };

struct SElem { CCharString m_name; long m_data[2]; CCharString m_label; CRetailVector<CCharString> m_strings; };
static void DestroyAux(SElem* first, SElem* last, const SFalseType&)
{
    for (; first != last; ++first)
        first->~SElem();
}

int main()
{
    SFalseType tag;
    SElem* recs = (SElem*)std::malloc(sizeof(SElem) * 2);
    for (int i = 0; i < 2; ++i) { new (&recs[i]) SElem(); MakeVector(recs[i].m_strings, i * 3); }   // 0, 3 strings
    DestroyAux(recs, recs + 2, tag);
    bool ok = g_stringsDestroyed == 2 * 2 + 3 && g_frees == 1;
    std::free(recs);
    if (!ok) { std::printf("00cdc000_TEST FAIL\n"); return 1; }
    std::printf("00cdc000_TEST PASS\n");
    return 0;
}