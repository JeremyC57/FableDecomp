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

struct SVectorRecord14 { long m_key[2]; CRetailVector<long> m_values; };
struct SElem { unsigned long m_key; CRetailVector<SVectorRecord14> m_records; };
static void DestroyAux(SElem* first, SElem* last, const SFalseType&)
{
    for (; first != last; ++first)
        first->~SElem();
}

int main()
{
    SFalseType tag;
    SElem* recs = (SElem*)std::malloc(sizeof(SElem) * 2);
    new (&recs[0]) SElem(); MakeVector(recs[0].m_records, 2);
    MakeVector(recs[0].m_records._M_start[0].m_values, 3);
    MakeVector(recs[0].m_records._M_start[1].m_values, 0);
    new (&recs[1]) SElem(); MakeVector(recs[1].m_records, 1);
    MakeVector(recs[1].m_records._M_start[0].m_values, 1);
    DestroyAux(recs, recs + 2, tag);
    // inner storage: two non-empty value vectors; outer storage: two record vectors
    bool ok = g_frees == 4;
    std::free(recs);
    if (!ok) { std::printf("00722130_TEST FAIL\n"); return 1; }
    std::printf("00722130_TEST PASS\n");
    return 0;
}