#include <cstdio>
#include <new>
static int g_strCopies = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
};
struct CProfilingInfo {
    bool m_enabled;
    CCharString m_name;
    long m_time;
    CProfilingInfo(const char* s) : m_enabled(true), m_name(s), m_time(250) { }
};
struct CLongProfilingPair {
    const long first;
    CProfilingInfo second;
    CLongProfilingPair(long k, const char* s) : first(k), second(s) { }
};
void Construct_PairLongProfilingInfo_00769270(CLongProfilingPair* p, const CLongProfilingPair& value)
{
    if (p) new (p) CLongProfilingPair(value);   // VC null-tests the placement result itself
}
int main()
{
    CLongProfilingPair src(42, "Render");
    static double storage[8];
    CLongProfilingPair* dst = (CLongProfilingPair*)(void*)storage;
    Construct_PairLongProfilingInfo_00769270(dst, src);
    if (g_strCopies != 1 || dst->first != 42 || !dst->second.m_enabled || dst->second.m_name.m_pData != src.second.m_name.m_pData
        || dst->second.m_time != 250) { std::printf("00769270_TEST FAIL\n"); return 1; }
    Construct_PairLongProfilingInfo_00769270(0, src);
    if (g_strCopies != 1) { std::printf("00769270_TEST FAIL\n"); return 1; }
    std::printf("00769270_TEST PASS\n");
    return 0;
}