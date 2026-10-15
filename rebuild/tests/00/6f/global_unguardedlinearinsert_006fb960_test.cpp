#include <cstdio>
struct CEvent { long m_a, m_b; };
static int g_live = 0;
struct CSmallVector {
    CEvent* m_pData; unsigned long m_count;
    CSmallVector(unsigned long n, long seed) : m_pData(n ? new CEvent[n] : 0), m_count(n)
    { if (m_pData) ++g_live; for (unsigned long i = 0; i < n; ++i) { m_pData[i].m_a = seed; m_pData[i].m_b = (long)i; } }
    void Copy(const CSmallVector& other)
    {
        if (m_pData) --g_live;
        delete[] m_pData; m_pData = 0; m_count = 0;
        unsigned long n = other.m_count;
        m_pData = new CEvent[n]; ++g_live; m_count = n;
        for (unsigned long i = 0; i < other.m_count; ++i) m_pData[i] = other.m_pData[i];
    }
    CSmallVector(const CSmallVector& other) : m_pData(0), m_count(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other) { if (&other != this) Copy(other); return *this; }
    ~CSmallVector() { if (m_pData) --g_live; delete[] m_pData; }
};
struct SPair {
    unsigned long first; CSmallVector second;
    SPair(unsigned long k, long seed) : first(k), second(1, seed) { }
    SPair& operator=(const SPair& other) { first = other.first; second = other.second; return *this; }
};
struct CLess { bool operator()(unsigned long a, const SPair& b) const { return a < b.first; } };
static void UnguardedLinearInsert(SPair* last, SPair value, CLess comp)
{
    unsigned long key = value.first;
    SPair* next = last;
    --next;
    while (comp(key, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    last->first = key;
    last->second = value.second;
}
int main()
{
    bool ok = true;
    {
        SPair a[4] = { SPair(1, 100), SPair(5, 500), SPair(9, 900), SPair(3, 300) };
        UnguardedLinearInsert(&a[3], a[3], CLess());
        ok = a[0].first == 1 && a[1].first == 3 && a[2].first == 5 && a[3].first == 9
            && a[1].second.m_pData[0].m_a == 300 && a[2].second.m_pData[0].m_a == 500 && a[3].second.m_pData[0].m_a == 900;
        UnguardedLinearInsert(&a[3], SPair(10, 1000), CLess());     // already in place: only the final store
        ok = ok && a[3].first == 10 && a[3].second.m_pData[0].m_a == 1000 && a[2].first == 5;
    }
    ok = ok && g_live == 0;
    if (!ok) { std::printf("006fb960_TEST FAIL\n"); return 1; }
    std::printf("006fb960_TEST PASS\n");
    return 0;
}