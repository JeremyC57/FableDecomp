#include <cstdio>
#include <cstdlib>
#include <cstring>
class CWideString;
static int g_calls = 0; static unsigned long g_a, g_b;
class CBankFile { public:
    virtual void v00() {}
    unsigned char _pad[0x148 - 4];
    void OpenFileReadWrite(const CWideString*, unsigned long a, unsigned long b) { ++g_calls; g_a = a; g_b = b; }
};
class CBankFileAsync : public CBankFile { public:
    bool m_asyncReading; bool m_asyncOpen;
    CBankFileAsync() : m_asyncReading(false), m_asyncOpen(false) {}
    virtual void OpenFileReadWrite(const CWideString* name, unsigned long a, unsigned long b) { CBankFile::OpenFileReadWrite(name, a, b); m_asyncOpen = true; }
};
static bool Check()
{
    CBankFileAsync bank; bank.OpenFileReadWrite(0, 3, 16);
    return g_calls == 1 && g_a == 3 && g_b == 16 && bank.m_asyncOpen && !bank.m_asyncReading;
}
int main()
{
    if (!Check()) { std::printf("009d5200_TEST FAIL\n"); return 1; }
    std::printf("009d5200_TEST PASS\n");
    return 0;
}