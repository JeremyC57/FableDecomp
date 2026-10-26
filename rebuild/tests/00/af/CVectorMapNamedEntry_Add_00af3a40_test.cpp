#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct SNamedEntry { long m_word0; long m_word4; CCharString m_name; SNamedEntry(long a, long b, const char* n) : m_word0(a), m_word4(b), m_name(n) { } };
struct SNamedEntryPair {
    CCharString first; SNamedEntry second;
    SNamedEntryPair(const CCharString& key, const SNamedEntry& value) : first(key), second(value) { }
};
struct SNamedEntryPairVector {
    const char* m_key[4]; const char* m_name[4]; long m_w4[4]; int m_count;
    void PushBack(const SNamedEntryPair& p) { m_key[m_count] = p.first.m_pData; m_name[m_count] = p.second.m_name.m_pData; m_w4[m_count] = p.second.m_word4; ++m_count; }
};
struct CVectorMap_NamedEntry {
    SNamedEntryPairVector m_vector; unsigned char Compare; bool Dirty;
    void Add(const CCharString& key, const SNamedEntry& value)
    {
        {
            SNamedEntryPair pair(key, value);
            m_vector.PushBack(pair);
        }
        Dirty = true;
    }
};
int main()
{
    CVectorMap_NamedEntry m; m.m_vector.m_count = 0; m.Dirty = false;
    {
        CCharString key("K"); SNamedEntry e(1, 2, "N");
        m.Add(key, e);
        if (g_live != 2) { std::printf("00af3a40_TEST FAIL\n"); return 1; }
    }
    if (m.m_vector.m_count != 1 || std::strcmp(m.m_vector.m_key[0], "K") != 0 || std::strcmp(m.m_vector.m_name[0], "N") != 0 || m.m_vector.m_w4[0] != 2 || !m.Dirty || g_live != 0) { std::printf("00af3a40_TEST FAIL\n"); return 1; }
    std::printf("00af3a40_TEST PASS\n");
    return 0;
}