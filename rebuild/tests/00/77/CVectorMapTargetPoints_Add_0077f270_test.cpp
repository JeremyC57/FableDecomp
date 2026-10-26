#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct STargetPoint {
    long m_word0, m_word4, m_word8;
    STargetPoint(long a, long b, long c) : m_word0(a), m_word4(b), m_word8(c) { }
    STargetPoint(const STargetPoint& o) : m_word0(o.m_word0), m_word4(o.m_word4), m_word8(o.m_word8) { }
};
struct STargetPointPair {
    CCharString first; STargetPoint second;
    STargetPointPair(const CCharString& key, const STargetPoint& value) : first(key), second(value) { }
};
struct CTargetPointPairVector {
    const char* m_names[4]; long m_w8[4]; int m_count;
    void PushBack(const STargetPointPair& p) { m_names[m_count] = p.first.m_pData; m_w8[m_count] = p.second.m_word8; ++m_count; }
};
struct CVectorMap_TargetPoints {
    CTargetPointPairVector m_vector; unsigned char Compare; bool Dirty;
    void Add(const CCharString& key, const STargetPoint& value)
    {
        {
            STargetPointPair pair(key, value);
            m_vector.PushBack(pair);
        }
        Dirty = true;
    }
};
int main()
{
    CVectorMap_TargetPoints m; m.m_vector.m_count = 0; m.Dirty = false;
    {
        CCharString name("GATE"); STargetPoint p(1, 2, 3);
        m.Add(name, p);
        if (g_live != 1) { std::printf("0077f270_TEST FAIL\n"); return 1; }
    }
    if (m.m_vector.m_count != 1 || std::strcmp(m.m_vector.m_names[0], "GATE") != 0 || m.m_vector.m_w8[0] != 3 || !m.Dirty || g_live != 0) { std::printf("0077f270_TEST FAIL\n"); return 1; }
    std::printf("0077f270_TEST PASS\n");
    return 0;
}