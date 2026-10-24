#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "005f9eea"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

static int g_pushes = 0; static LONG32 g_lastFirst = 0, g_lastSecond = 0;
struct SLongPair {
    LONG32 first;
    LONG32 second;
    SLongPair(const LONG32& key, const LONG32& value) : first(key), second(value) { }
};
struct CLongPairVector {
    void push_back(const SLongPair& pair) { ++g_pushes; g_lastFirst = pair.first; g_lastSecond = pair.second; }
};
struct CVectorMap {
    CLongPairVector m_vector;
    unsigned char   Compare;
    bool            Dirty;
    void Add(const LONG32& key, const LONG32& value)
    {
        m_vector.push_back(SLongPair(key, value));
        Dirty = true;
    }
};
int main()
{
    CVectorMap m; m.Dirty = false;
    LONG32 k = 4, v = 77;
    m.Add(k, v);
    CHECK(g_pushes == 1 && g_lastFirst == 4 && g_lastSecond == 77 && m.Dirty);
    std::printf("005f9eea_TEST PASS\n");
    return 0;
}