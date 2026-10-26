#include <cstdio>
#include <cstdlib>
#include <new>

static int g_frees = 0;
static int g_stringsDestroyed = 0;
static void CountingFree(void* p) { if (p) ++g_frees; std::free(p); }

class CCharString { public:
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

struct SNamedRecordC { long m_key; CCharString m_name; long m_value; };
struct SElem { unsigned long m_key; CCharString m_name; long m_value; CRetailVector<SNamedRecordC> m_records; };
static void DestroyAux(SElem* first, SElem* last, const SFalseType&)
{
    for (; first != last; ++first)
        first->~SElem();
}

int main()
{
    SFalseType tag;
    SElem* recs = (SElem*)std::malloc(sizeof(SElem) * 3);
    for (int i = 0; i < 3; ++i) { new (&recs[i]) SElem(); MakeVector(recs[i].m_records, i); }   // 0, 1, 2 inner records
    DestroyAux(recs, recs + 3, tag);
    // 3 outer names + 0+1+2 inner names; storage freed for the two non-empty vectors
    bool ok = g_stringsDestroyed == 6 && g_frees == 2;
    g_stringsDestroyed = 0; DestroyAux(recs, recs, tag);
    if (g_stringsDestroyed != 0) ok = false;
    std::free(recs);
    if (!ok) { std::printf("0047c800_TEST FAIL\n"); return 1; }
    std::printf("0047c800_TEST PASS\n");
    return 0;
}