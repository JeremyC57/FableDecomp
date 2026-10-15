#include <cstdio>
#include <cstring>
static int g_live = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct SStringPair { CCharString m_first; CCharString m_second; SStringPair(const char* a, const char* b) : m_first(a), m_second(b) { } };
struct SStringPairKeyPair {
    SStringPair first; long second;
    SStringPairKeyPair(const SStringPair& key, const long& value) : first(key), second(value) { }
};
struct SStringPairKeyPairVector {
    const char* m_a[4]; const char* m_b[4]; long m_v[4]; int m_count;
    void PushBack(const SStringPairKeyPair& p) { m_a[m_count] = p.first.m_first.m_pData; m_b[m_count] = p.first.m_second.m_pData; m_v[m_count] = p.second; ++m_count; }
};
struct CVectorMap_StringPairKey {
    SStringPairKeyPairVector m_vector; unsigned char Compare; bool Dirty;
    void Add(const SStringPair& key, const long& value)
    {
        {
            SStringPairKeyPair pair(key, value);
            m_vector.PushBack(pair);
        }
        Dirty = true;
    }
};
int main()
{
    CVectorMap_StringPairKey m; m.m_vector.m_count = 0; m.Dirty = false;
    {
        SStringPair key("A", "B"); long v = 9;
        m.Add(key, v);
        if (g_live != 2) { std::printf("00a41a70_TEST FAIL\n"); return 1; }
    }
    if (m.m_vector.m_count != 1 || std::strcmp(m.m_vector.m_a[0], "A") != 0 || std::strcmp(m.m_vector.m_b[0], "B") != 0 || m.m_vector.m_v[0] != 9 || !m.Dirty || g_live != 0) { std::printf("00a41a70_TEST FAIL\n"); return 1; }
    std::printf("00a41a70_TEST PASS\n");
    return 0;
}
