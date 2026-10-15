#include <cstdio>
#include <cstring>
struct CEvent { long a, b; };
static int g_frees = 0;
struct CSmallVector_CEvent32 {
    CEvent* m_pData; unsigned long m_size;
    CSmallVector_CEvent32() : m_pData(0), m_size(0) { }
    CSmallVector_CEvent32(const CSmallVector_CEvent32& other) : m_pData(0), m_size(0) { Copy(other); }
    ~CSmallVector_CEvent32() { if (m_pData) ++g_frees; delete[] m_pData; }
    void Copy(const CSmallVector_CEvent32& other)
    {
        delete[] m_pData; m_pData = 0; m_size = 0;
        m_pData = new CEvent[other.m_size ? other.m_size : 1]; m_size = other.m_size;
        for (unsigned long i = 0; i < m_size; ++i) m_pData[i] = other.m_pData[i];
    }
};
struct SAnimEventsPair {
    unsigned long first; CSmallVector_CEvent32 second;
    SAnimEventsPair(const unsigned long& key, const CSmallVector_CEvent32& value) : first(key), second(value) { }
};
struct CAnimEventsPairVector {
    unsigned long m_keys[4]; long m_firstEventA[4]; int m_count;
    void PushBack(const SAnimEventsPair& p) { m_keys[m_count] = p.first; m_firstEventA[m_count] = p.second.m_size ? p.second.m_pData[0].a : -1; ++m_count; }
};
struct CVectorMap_AnimEvents {
    CAnimEventsPairVector m_vector; unsigned char Compare; bool Dirty;
    void Add(const unsigned long& key, const CSmallVector_CEvent32& value)
    {
        {
            SAnimEventsPair pair(key, value);
            m_vector.PushBack(pair);
        }
        Dirty = true;
    }
};
int main()
{
    CVectorMap_AnimEvents m; m.m_vector.m_count = 0; m.Dirty = false;
    CSmallVector_CEvent32 ev; ev.m_pData = new CEvent[2]; ev.m_size = 2; ev.m_pData[0].a = 11; ev.m_pData[1].a = 12;
    m.Add(5, ev);
    if (m.m_vector.m_count != 1 || m.m_vector.m_keys[0] != 5 || m.m_vector.m_firstEventA[0] != 11 || !m.Dirty || g_frees != 1) { std::printf("006fcf40_TEST FAIL\n"); return 1; }
    std::printf("006fcf40_TEST PASS\n");
    return 0;
}
