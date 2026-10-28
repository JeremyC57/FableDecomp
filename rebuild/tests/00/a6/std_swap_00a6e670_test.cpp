// behaviour test for 0x00A6E670 std::swap<pair<CCharString, C3DDummyObject>>
#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    char* m_pData;
    CCharString(const char* s) { m_pData = new char[std::strlen(s) + 1]; std::strcpy(m_pData, s); ++g_live; }
    CCharString(const CCharString& o) { m_pData = new char[std::strlen(o.m_pData) + 1]; std::strcpy(m_pData, o.m_pData); ++g_live; }
    ~CCharString() { delete[] m_pData; --g_live; }
    const CCharString& operator=(const CCharString& o)
    { if (this != &o) { char* p = new char[std::strlen(o.m_pData) + 1]; std::strcpy(p, o.m_pData); delete[] m_pData; m_pData = p; } return *this; }
};
class C3DDummyObject { public: unsigned int m_words[13]; };
struct SDummyObjectEntry { CCharString first; C3DDummyObject second; SDummyObjectEntry(const char* n) : first(n) {} };
template <class T> void swap_(T& a, T& b) { T tmp(a); a = b; b = tmp; }
int main()
{
    {
        SDummyObjectEntry a("left"), b("right");
        for (int i = 0; i < 13; ++i) { a.second.m_words[i] = i; b.second.m_words[i] = 100 + i; }
        swap_(a, b);
        int bad = std::strcmp(a.first.m_pData, "right") != 0 || std::strcmp(b.first.m_pData, "left") != 0
            || a.second.m_words[12] != 112 || b.second.m_words[0] != 0 || g_live != 2;
        if (bad) { std::printf("00a6e670_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("00a6e670_TEST FAIL\n"); return 1; }
    std::printf("00a6e670_TEST PASS\n");
    return 0;
}