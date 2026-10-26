#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct SRecord16 {
    long m_word0, m_word4, m_word8, m_wordC;
    SRecord16(long a, long b, long c, long d) : m_word0(a), m_word4(b), m_word8(c), m_wordC(d) { }
    SRecord16(const SRecord16& o) : m_word0(o.m_word0), m_word4(o.m_word4), m_word8(o.m_word8), m_wordC(o.m_wordC) { }
};
struct SNamedRecord16Pair {
    CCharString first; SRecord16 second;
    SNamedRecord16Pair(const CCharString& key, const SRecord16& value) : first(key), second(value) { }
};
struct SNamedRecord16PairVector {
    const char* m_names[4]; long m_wC[4]; int m_count;
    void PushBack(const SNamedRecord16Pair& p) { m_names[m_count] = p.first.m_pData; m_wC[m_count] = p.second.m_wordC; ++m_count; }
};
struct CVectorMap_NamedRecord16 {
    SNamedRecord16PairVector m_vector; unsigned char Compare; bool Dirty;
    void Add(const CCharString& key, const SRecord16& value)
    {
        {
            SNamedRecord16Pair pair(key, value);
            m_vector.PushBack(pair);
        }
        Dirty = true;
    }
};
int main()
{
    CVectorMap_NamedRecord16 m; m.m_vector.m_count = 0; m.Dirty = false;
    {
        CCharString name("FULL"); SRecord16 r(1, 2, 3, 4);
        m.Add(name, r);
        if (g_live != 1) { std::printf("00a72b90_TEST FAIL\n"); return 1; }
    }
    if (m.m_vector.m_count != 1 || std::strcmp(m.m_vector.m_names[0], "FULL") != 0 || m.m_vector.m_wC[0] != 4 || !m.Dirty || g_live != 0) { std::printf("00a72b90_TEST FAIL\n"); return 1; }
    std::printf("00a72b90_TEST PASS\n");
    return 0;
}